#ifndef _RTESETTINGSVIDEOGUI_
#define _RTESETTINGSVIDEOGUI_

namespace RTE {

	class GUIControlManager;
	class GUICollectionBox;
	class GUIComboBox;
	class GUILabel;
	class GUIButton;
	class GUIRadioButton;
	class GUICheckbox;
	class GUIEvent;

	/// <summary>
	/// Handling for video settings through the game settings user interface.
	/// </summary>
	class SettingsVideoGUI {

	public:

#pragma region Creation
		/// <summary>
		/// Constructor method used to instantiate a SettingsVideoGUI object in system memory and make it ready for use.
		/// </summary>
		/// <param name="parentControlManager">Pointer to the parent GUIControlManager which owns all the GUIControls of this SettingsVideoGUI.</param>
		explicit SettingsVideoGUI(GUIControlManager *parentControlManager);
#pragma endregion

#pragma region Concrete Methods
		/// <summary>
		/// Enables or disables the video settings menu.
		/// </summary>
		/// <param name="enable">Show and enable or hide and disable the video settings menu.</param>
		void SetEnabled(bool enable = true) const;

		/// <summary>
		/// Handles the player interaction with the SettingsVideoGUI GUI elements.
		/// </summary>
		/// <param name="guiEvent">The GUIEvent containing information about the player interaction with an element.</param>
		void HandleInputEvents(GUIEvent &guiEvent);
#pragma endregion

	private:

		/// <summary>
		/// Enumeration for the different types of quick resolution change options.
		/// </summary>
		enum ResolutionQuickChangeType {
			Windowed,
			Borderless,
			UpscaledBorderless,
			Dedicated,
			UpscaledDedicated,
			QuickChangeTypeCount
		};

		/// <summary>
		/// Struct containing information about a supported resolution preset.
		/// </summary>
		struct PresetResolutionRecord {
			int Width; //!< Resolution width.
			int Height; //!< Resolution height.
			bool Upscaled; //!< Whether resolution is upscaled.

			/// <summary>
			/// Constructor method to instantiate a PresetResolutionRecord object in system memory and make it ready for use.
			/// </summary>
			/// <param name="width">Resolution width.</param>
			/// <param name="height">Resolution height.</param>
			/// <param name="upscaled">Whether resolution is upscaled.</param>
			PresetResolutionRecord(int width, int height, bool upscaled) : Width(width), Height(height), Upscaled(upscaled) {}

			/// <summary>
			/// Makes UI displayable string with resolution info.
			/// </summary>
			/// <returns>String with resolution info.</returns>
			std::string MakeResolutionString() const { return std::to_string(Width) + "x" + std::to_string(Height) + (Upscaled ? " Upscaled (" + std::to_string(Width / 2) + "x" + std::to_string(Height / 2) + ")" : ""); }

			/// <summary>
			/// Comparison operator for eliminating duplicates and sorting in the temporary PresetResolutionRecord std::sets during PopulateResolutionsComboBox.
			/// </summary>
			/// <param name="rhs">The PresetResolutionRecord to compare with.</param>
			/// <returns>Bool with the result of the comparison.</returns>
			bool operator<(const PresetResolutionRecord &rhs) const {
				if (Width == rhs.Width && Height == rhs.Height) {
					return Upscaled != rhs.Upscaled;
				} else if (Width == rhs.Width) {
					return Height < rhs.Height;
				}
				return Width < rhs.Width;
			}
		};

		GUIControlManager *m_GUIControlManager; //!< The GUIControlManager which holds all the GUIControls of this menu. Not owned by this.

		std::vector<PresetResolutionRecord> m_PresetResolutions; //!< Contains PresetResolutionRecords for all the supported preset resolutions.

		int m_NewGraphicsDriver; //!< The new graphics driver to use when changing resolution.
		int m_NewResX; //!< The new resolution width to use when changing resolution.
		int m_NewResY; //!< The new resolution height to use when changing resolution.
		bool m_NewResUpscaled; //!< Whether the new resolution should be upscaled when changing resolution.

		/// <summary>
		/// GUI elements that compose the video settings menu screen.
		/// </summary>
		GUICollectionBox *m_VideoSettingsBox;
		GUIRadioButton *m_PresetResolutionRadioButton;
		GUIRadioButton *m_CustomResolutionRadioButton;
		GUICollectionBox *m_PresetResolutionBox;
		GUIComboBox *m_PresetResolutionComboBox;
		GUIButton *m_PresetResolutionApplyButton;
		GUILabel *m_PresetResolutionMessageLabel;
		GUICollectionBox *m_CustomResolutionBox;
		GUIButton *m_CustomResolutionApplyButton;
		GUICollectionBox *m_ResolutionChangeDialogBox;
		GUIButton *m_ConfirmResolutionChangeButton;
		GUIButton *m_CancelResolutionChangeButton;
		std::array<GUIButton *, ResolutionQuickChangeType::QuickChangeTypeCount> m_ResolutionQuickToggleButtons;

#pragma region Video Settings Handling
		/// <summary>
		/// Checks whether the passed in width and height values make a supported resolution setting.
		/// </summary>
		/// <param name="width">Resolution width.</param>
		/// <param name="height">Resolution height.</param>
		/// <returns>Whether the resolution is supported or not.</returns>
		bool IsSupportedResolution(int width, int height) const;

		/// <summary>
		/// Fills the PresetResolutions list with all valid PresetResolutionRecords, then fills the PresetResolutionComboBox using it and selects the currently selected preset resolution, if any.
		/// </summary>
		void PopulateResolutionsComboBox();

		/// <summary>
		/// 
		/// </summary>
		void ApplyNewResolution();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="resolutionChangeType"></param>
		void ApplyQuickChangeResolution(ResolutionQuickChangeType resolutionChangeType);

		/// <summary>
		/// 
		/// </summary>
		void ApplyPresetResolution();

		/// <summary>
		/// 
		/// </summary>
		void ApplyCustomResolution();
#pragma endregion

		// Disallow the use of some implicit methods.
		SettingsVideoGUI(const SettingsVideoGUI &reference) = delete;
		SettingsVideoGUI & operator=(const SettingsVideoGUI &rhs) = delete;
	};
}
#endif