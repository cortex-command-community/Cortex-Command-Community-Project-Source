#include "Leg.h"
#include "PresetMan.h"

namespace RTE {

	ConcreteClassInfo(Leg, Attachable, 50)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::Clear() {
		m_Foot = 0;

		m_ContractedOffset.Reset();
		m_ExtendedOffset.Reset();

		m_MinExtension = 0;
		m_MaxExtension = 0;
		m_CurrentNormalizedExtension = 0;

		m_TargetPosition.Reset();
		m_IdleOffset.Reset();

		m_CurrentAnkleOffset.Reset();

		m_WillIdle = false;
		m_MoveSpeed = 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Leg::Create() {
		if (MOSRotating::Create() < 0) {
			return -1;
		}

		// Ensure Legs don't get flagged as inheriting RotAngle, since they never do and always set their RotAngle for themselves.
		m_InheritsRotAngle = false;

		// Ensure Legs don't collide with terrain when attached since their expansion/contraction is frame based so atom group doesn't know how to account for it.
		SetCollidesWithTerrainWhileAttached(false);

		if (m_ContractedOffset.GetMagnitude() > m_ExtendedOffset.GetMagnitude()) {
			std::swap(m_ContractedOffset, m_ExtendedOffset);
		}

		m_MinExtension = m_ContractedOffset.GetMagnitude();
		m_MaxExtension = m_ExtendedOffset.GetMagnitude();

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Leg::Create(const Leg &reference) {
		if (reference.m_Foot) {
			m_ReferenceHardcodedAttachableUniqueIDs.insert(reference.m_Foot->GetUniqueID());
			SetFoot(dynamic_cast<Attachable *>(reference.m_Foot->Clone()));
		}
		Attachable::Create(reference);

		m_ContractedOffset = reference.m_ContractedOffset;
		m_ExtendedOffset = reference.m_ExtendedOffset;

		m_MinExtension = reference.m_MinExtension;
		m_MaxExtension = reference.m_MaxExtension;
		m_CurrentNormalizedExtension = reference.m_CurrentNormalizedExtension;

		m_TargetPosition = reference.m_TargetPosition;
		m_IdleOffset = reference.m_IdleOffset;

		m_CurrentAnkleOffset = reference.m_CurrentAnkleOffset;

		m_WillIdle = reference.m_WillIdle;
		m_MoveSpeed = reference.m_MoveSpeed;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Leg::ReadProperty(std::string propName, Reader &reader) {
		if (propName == "Foot") {
			RemoveAttachable(m_Foot);
			const Entity *footEntity = g_PresetMan.GetEntityPreset(reader);
			if (footEntity) {
				m_Foot = dynamic_cast<Attachable *>(footEntity->Clone());
				AddAttachable(m_Foot);
				m_Foot->SetInheritsRotAngle(false);
				m_Foot->SetParentGibBlastStrengthMultiplier(0.0F);
				m_Foot->SetCollidesWithTerrainWhileAttached(false);
			}
		} else if (propName == "ContractedOffset") {
			reader >> m_ContractedOffset;
			m_MinExtension = m_ContractedOffset.GetMagnitude();
		} else if (propName == "ExtendedOffset") {
			reader >> m_ExtendedOffset;
			m_MaxExtension = m_ExtendedOffset.GetMagnitude();
		} else if (propName == "IdleOffset") {
			reader >> m_IdleOffset;
		} else if (propName == "WillIdle") {
			reader >> m_WillIdle;
		} else if (propName == "MoveSpeed") {
			reader >> m_MoveSpeed;
		} else {
			return Attachable::ReadProperty(propName, reader);
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Leg::Save(Writer &writer) const {
		Attachable::Save(writer);

		writer.NewProperty("Foot");
		writer << m_Foot;
		writer.NewProperty("ContractedOffset");
		writer << m_ContractedOffset;
		writer.NewProperty("ExtendedOffset");
		writer << m_ExtendedOffset;
		writer.NewProperty("IdleOffset");
		writer << m_IdleOffset;
		writer.NewProperty("WillIdle");
		writer << m_WillIdle;
		writer.NewProperty("MoveSpeed");
		writer << m_MoveSpeed;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::SetFoot(Attachable *newFoot) {
		if (newFoot == nullptr) {
			if (m_Foot && m_Foot->IsAttachedTo(this)) { RemoveAttachable(m_Foot); }
			m_Foot = nullptr;
		} else {
			RemoveAttachable(m_Foot);
			m_Foot = newFoot;
			AddAttachable(newFoot);
			m_HardcodedAttachableUniqueIDsAndSetters.insert({newFoot->GetUniqueID(), [](MOSRotating *parent, Attachable *attachable) { dynamic_cast<Leg *>(parent)->SetFoot(attachable); }});
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::Update() {
		UpdateCurrentAnkleOffsetAndFootParentOffset();

		Attachable::Update();

		UpdateLegRotation();

		UpdateLegFrameAndNormalizedExtension();

		UpdateFootFrameAndRotation();
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::UpdateCurrentAnkleOffsetAndFootParentOffset() {
		if (IsAttached()) {
			Vector targetOffset = g_SceneMan.ShortestDistance(m_JointPos, m_TargetPosition, g_SceneMan.SceneWrapsX());
			if (m_WillIdle && targetOffset.m_Y < -3) {
				targetOffset = m_IdleOffset.GetXFlipped(m_HFlipped);
			}

			Vector distanceFromTargetOffsetToAnkleOffset(targetOffset - m_CurrentAnkleOffset);
			m_CurrentAnkleOffset += distanceFromTargetOffsetToAnkleOffset * m_MoveSpeed;
			m_CurrentAnkleOffset.ClampMagnitude(m_MaxExtension, m_MinExtension + 0.1F);
		} else {
			m_CurrentAnkleOffset.SetXY(m_MaxExtension * 0.60F, 0);
			m_CurrentAnkleOffset.RadRotate((m_HFlipped ? c_PI : 0) + m_Rotation.GetRadAngle());
		}

		if (m_Foot) {
			// In order to keep the foot in the right place, we need to convert its offset (the ankle offstet) to work as the ParentOffset for the foot.
			// The foot will then use this to set its JointPos when it's updated. Unfortunately UnRotateOffset doesn't work for this, since it's Vector/Matrix division, which isn't commutative.
			Vector ankleOffsetAsParentOffset = RotateOffset(m_JointOffset) + m_CurrentAnkleOffset;
			ankleOffsetAsParentOffset.RadRotate(-m_Rotation.GetRadAngle()).FlipX(m_HFlipped);
			m_Foot->SetParentOffset(ankleOffsetAsParentOffset);
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::UpdateLegRotation() {
		if (IsAttached()) {
			m_Rotation = m_CurrentAnkleOffset.GetAbsRadAngle() + (m_HFlipped ? c_PI : 0);

			// Get a normalized scalar for where the Leg should be rotated to between the contracted and extended offsets. EaseOut is used to get the sine effect needed.
			float extraRotationRatio = (EaseOut(m_MinExtension, m_MaxExtension, m_CurrentNormalizedExtension) - m_MinExtension) / (m_MaxExtension - m_MinExtension);

			// The contracted offset's inverse angle is the base for the rotation correction.
			float extraRotation = -(m_ContractedOffset.GetAbsRadAngle());

			// Get the actual amount of extra rotation correction needed from the ratio, somewhere on the arc between contracted and extended angles.
			// This is negative because it's a correction, the bitmap needs to rotate back to align the ankle with where it's supposed to be in the sprite.
			extraRotation -= (m_ExtendedOffset.GetAbsRadAngle() - m_ContractedOffset.GetAbsRadAngle()) * extraRotationRatio;

			m_Rotation.SetRadAngle(m_Rotation.GetRadAngle() + extraRotation * static_cast<float>(GetFlipFactor()));
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::UpdateLegFrameAndNormalizedExtension() {
		if (m_FrameCount == 1) {
			m_Frame = 0;
		} else {
			m_CurrentNormalizedExtension = Limit((m_CurrentAnkleOffset.GetMagnitude() - m_MinExtension) / (m_MaxExtension - m_MinExtension), 1.0F, 0.0F);
			m_Frame = std::min(m_FrameCount - 1, static_cast<unsigned int>(std::floor(m_CurrentNormalizedExtension * static_cast<float>(m_FrameCount))));
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Leg::UpdateFootFrameAndRotation() {
		if (m_Foot) {
			if (IsAttached()) {
				float ankleOffsetHorizontalDistanceAccountingForFlipping = m_CurrentAnkleOffset.GetXFlipped(m_HFlipped).GetX();
				if (ankleOffsetHorizontalDistanceAccountingForFlipping < -10) {
					m_Foot->SetFrame(3);
				} else if (ankleOffsetHorizontalDistanceAccountingForFlipping < -6) {
					m_Foot->SetFrame(2);
				} else if (ankleOffsetHorizontalDistanceAccountingForFlipping > 6) {
					m_Foot->SetFrame(1);
				} else {
					m_Foot->SetFrame(0);
				}
				m_Foot->SetRotAngle(0.0F);
			} else {
				m_Foot->SetRotAngle(m_Rotation.GetRadAngle() + c_PI / 2);
			}
		}
	}
}