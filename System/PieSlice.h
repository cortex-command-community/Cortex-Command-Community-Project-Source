#ifndef _PIESLICE_
#define _PIESLICE_

#include "Serializable.h"
#include "Icon.h"

namespace RTE {

	/// <summary>
	/// An individual slice in a pie menu.
	/// </summary>
	class PieSlice : public Serializable {

	public:

		SerializableOverrideMethods
		SerializableClassNameGetter

		enum PieSliceIndex {
			PSI_NONE = 0,
			// Inventory management
			PSI_PICKUP,
			PSI_DROP,
			PSI_NEXTITEM,
			PSI_PREVITEM,
			PSI_RELOAD,
			// Menu and GUI activations
			PSI_BUYMENU,
			PSI_STATS,
			PSI_MINIMAP,
			PSI_FORMSQUAD,
			PSI_CEASEFIRE,
			// AI Modes
			PSI_SENTRY,
			PSI_PATROL,
			PSI_BRAINHUNT,
			PSI_GOLDDIG,
			PSI_GOTO,
			PSI_RETURN,
			PSI_STAY,
			PSI_DELIVER,
			PSI_SCUTTLE,
			// Editor stuff
			PSI_DONE,
			PSI_LOAD,
			PSI_SAVE,
			PSI_NEW,
			PSI_PICK,
			PSI_MOVE,
			PSI_REMOVE,
			PSI_INFRONT,
			PSI_BEHIND,
			PSI_ZOOMIN,
			PSI_ZOOMOUT,
			PSI_TEAM1,
			PSI_TEAM2,
			PSI_TEAM3,
			PSI_TEAM4,
			// Custom
			PSI_SCRIPTED,
			// How many pie slice types there are
			PSI_COUNT
		};

		/// <summary>
		/// An enum for the directions a slice can be placed in, in a given pie menu.
		/// </summary>
		enum SliceDirection {
			NONE = 0,
			UP,
			RIGHT,
			DOWN,
			LEFT
		};

#pragma region Creation
		/// <summary>
		/// Constructor method used to instantiate a Slice object in system memory. Create() should be called before using the object.
		/// </summary>
		PieSlice() { Clear(); }

		/// <summary>
		/// Constructor method used to instantiate a Slice object in system memory. Create() should be called before using the object.
		/// </summary>
		/// <param name="description">The description of the pie slice.</param>
		/// <param name="sliceType">The type of the pie slice.</param>
		/// <param name="direction">The direction of the pie slice.</param>
		/// <param name="enabled">Whether the slice is enabled. Defaults to true.</param>
		PieSlice(const std::string_view &description, PieSliceIndex sliceType, SliceDirection direction, bool enabled = true) { Clear(); m_Description = description; m_SliceType = sliceType; m_Direction = direction; m_Enabled = enabled; }
		//PieSlice(const std::string &description, PieSliceIndex sliceType, SliceDirection direction, bool enabled = true) : Clear(), m_Description(description), m_SliceType(sliceType), m_Direction(direction), m_Enabled(enabled) { Clear(); }

		/// <summary>
		/// Makes the Slice object ready for use.
		/// </summary>
		/// <returns>An error return value signaling sucess or any particular failure. Anything below 0 is an error signal.</returns>
		int Create() override;

		/// <summary>
		/// Creates a Slice to be identical to another, by deep copy.
		/// </summary>
		/// <param name="reference">A reference to the Slice to deep copy.</param>
		/// <returns>An error return value signaling sucess or any particular failure. Anything below 0 is an error signal.</returns>
		int Create(const PieSlice &reference);

		/// <summary>
		/// Makes the Serializable ready for use. Pie slices being read from a file will always be scripted SliceType.
		/// </summary>
		/// <param name="reader">A Reader that the Serializable will create itself from.</param>
		/// <param name="checkType">Whether there is a class name in the stream to check against to make sure the correct type is being read from the stream.</param>
		/// <param name="doCreate">Whether to do any additional initialization of the object after reading in all the properties from the Reader. This is done by calling Create().</param>
		/// <returns>An error return value signaling success or any particular failure. Anything below 0 is an error signal.</returns>
		int Create(Reader &reader, bool checkType = true, bool doCreate = true) override { m_SliceType = PieSliceIndex::PSI_SCRIPTED; return Serializable::Create(reader, checkType, doCreate); }
#pragma endregion

#pragma region Destruction
		/// <summary>
		/// Resets the entire Serializable, including its inherited members, to their default settings or values.
		/// </summary>
		void Reset() override { Clear(); }
#pragma endregion

#pragma region Getters and Setters
		/// <summary>
		/// Gets the type of this Slice.
		/// </summary>
		/// <returns>The type of this Slice.</returns>
		const PieSliceIndex &GetType() const { return m_SliceType; }

		/// <summary>
		/// Gets the description this Slice shows when selected in its pie menu.
		/// </summary>
		/// <returns>The description of this Slice.</returns>
		const std::string &GetDescription() const { return m_Description; }

		/// <summary>
		/// Gets the direction this Slice is set to be at in its pie menu.
		/// </summary>
		/// <returns>The direction of this Slice.</returns>
		const SliceDirection &GetDirection() const { return m_Direction; }

		/// <summary>
		/// Sets the direction this Slice should be at in its pie menu.
		/// </summary>
		/// <param name="direction">The direction to set for the Slice.</param>
		void SetDirection(SliceDirection direction) { m_Direction = direction; }

		/// <summary>
		/// Gets whether or not this Slice is enabled.
		/// </summary>
		/// <returns>Whether or not this Slice is enabled.</returns>
		bool IsEnabled() const { return m_Enabled; }

		/// <summary>
		/// Sets whether or not this Slice should be enabled.
		/// </summary>
		/// <param name="enabled">Whether or not this Slice should be enabled.</param>
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

		/// <summary>
		/// Gets the icon for this Slice.
		/// </summary>
		/// <returns>The icon for this Slice.</returns>
		const Icon & GetIcon() const { return m_Icon; }

		/// <summary>
		/// Gets the start angle this Slice's area is set to be at in its pie menu.
		/// </summary>
		/// <returns>The start angle of this Slice's area.</returns>
		float GetAreaStart() const { return m_AreaStart; }

		/// <summary>
		/// Sets the start angle this Slice's area should be at in its pie menu.
		/// </summary>
		/// <param name="areaStart">The start angle to set for the Slice's area.</param>
		void SetAreaStart(float areaStart) { m_AreaStart = areaStart; }

		/// <summary>
		/// Gets the arc length this of this Slice's area.
		/// </summary>
		/// <returns>The arc length of this Slice's area.</returns>
		float GetAreaArc() const { return m_AreaArc; }

		/// <summary>
		/// Sets the arc length of this Slice's area.
		/// </summary>
		/// <param name="areaArc">The arc length to set for the Slice's area.</param>
		void SetAreaArc(float areaArc) { m_AreaArc = areaArc; }

		/// <summary>
		/// Gets the mid angle this Slice's area is set to be at in its pie menu.
		/// </summary>
		/// <returns>The mid angle of this Slice's area.</returns>
		float GetMidAngle() const { return m_MidAngle; }

		/// <summary>
		/// Sets the mid angle this Slice's area should be at in its pie menu.
		/// </summary>
		/// <param name="midAngle">The mid angle to set for the Slice's area.</param>
		void SetMidAngle(float midAngle) { m_MidAngle = midAngle; }

		/// <summary>
		/// Gets the file path of the scripted file this Slice should run when activated. Empty if it's not a scripted type. Empty if its SliceType isn't scripted.
		/// </summary>
		/// <returns>The file path to the script file this Slice should load when activated.</returns>
		const std::string & GetScriptPath() const { return m_ScriptPath; }

		/// <summary>
		/// Gets the name of the Lua function to run when this Slice is activated as a scripted pie menu option. Empty if its SliceType isn't scripted.
		/// </summary>
		/// <returns>The Lua function name this Slice should execute when activated.</returns>
		const std::string & GetFunctionName() const { return m_FunctionName; }
#pragma endregion

	private:
		static const std::string c_ClassName; //!< ClassName for Slice.

		PieSliceIndex m_SliceType; //!< The Slice type, also serves as icon index.
		std::string m_Description; //!< Description of what this slice option does.
		SliceDirection m_Direction; //!< The desired direction/location of this on the pie menu.
		bool m_Enabled; //!< Whether this slice is enabled or disabled and greyed out.
		Icon m_Icon; //!< The icon of this pie slice.

		float m_AreaStart; //!< The start angle of this' area on the pie menu, counted in radians from straight out right and going counter clockwise.
		float m_AreaArc; //!< The arc length of the slice area, so that the icon should be drawn at the areastart + halfway of this.
		float m_MidAngle; //!< Mid angle, basically m_AreaStart + (m_AreaArc / 2).

		std::string m_ScriptPath; //!< Path to the script file this should run if this is a scripted selection.
		std::string m_FunctionName; //!< Name of the function in the script that this should run if a scripted pie option.

		/// <summary>
		/// Clears all the member variables of this Slice, effectively resetting the members of this abstraction level only.
		/// </summary>
		void Clear();
	};
}
#endif