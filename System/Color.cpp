#include "Color.h"

namespace RTE {

	const std::string Color::c_ClassName = "Color";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Color::Create() {
		// Read all the properties
		if (Serializable::Create() < 0) { 
			return -1; 
		}
		RecalculateIndex();
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Color::Create(unsigned char inputR, unsigned char inputG, unsigned char inputB) {
		m_R = inputR;
		m_G = inputG;
		m_B = inputB;
		RecalculateIndex();
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Color::ReadProperty(std::string propName, Reader &reader) {
		if (propName == "R") {
			reader >> m_R;
		} else if (propName == "G") {
			reader >> m_G;
		} else if (propName == "B") {
			reader >> m_B;
		} else {
			return Serializable::ReadProperty(propName, reader);
		}
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Color::Save(Writer &writer) const {
		Serializable::Save(writer);

		writer.NewProperty("R");
		writer << m_R;
		writer.NewProperty("G");
		writer << m_G;
		writer.NewProperty("B");
		writer << m_B;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Color::SetRGBWithIndex(unsigned char index) {

		m_Index = index;

		RGB color;
		get_color(static_cast<int>(m_Index), &color);

		// Multiply by 4 because the Allegro RGB struct elements are in range 0-63, and proper RGB needs 0-255.
		m_R = color.r * 4;
		m_G = color.g * 4;
		m_B = color.b * 4;
	}
}