/*

		class ModRecord {
		public:
			std::string ModulePath;
			std::string ModuleName;
			std::string Description;
			bool Disabled;

			bool operator<(const ModRecord &rhs) const { return ModulePath < rhs.ModulePath; }
		};

		class ScriptRecord {
		public:
			std::string PresetName;
			std::string Description;
			bool Enabled;

			bool operator<(const ScriptRecord &rhs) const { return PresetName < rhs.PresetName; }
		};


		/// <summary>
		/// Makes UI displayable string with mod info.
		/// </summary>
		/// <param name="r"></param>
		/// <returns>String with mod info.</returns>
		std::string MakeModString(ModRecord r);

		/// <summary>
		/// Makes UI displayable string with script info.
		/// </summary>
		/// <param name="r"></param>
		/// <returns>String with script info.</returns>
		std::string MakeScriptString(ScriptRecord r);

		/// <summary>
		/// Turns currently selected mod on and off and changes UI elements accordingly.
		/// </summary>
		void ToggleMod();

		/// <summary>
		/// Turns currently selected script on and off and changes UI elements accordingly.
		/// </summary>
		void ToggleScript();



		std::vector<ModRecord> m_KnownMods;
		std::vector<ScriptRecord> m_KnownScripts;

		GUIButton *m_pModManagerBackButton;
		GUIButton *m_pModManagerToggleModButton;
		GUIButton *m_pModManagerToggleScriptButton;
		GUIListBox *m_pModManagerModsListBox;
		GUIListBox *m_pModManagerScriptsListBox;
		GUILabel *m_pModManagerDescriptionLabel;




























*/