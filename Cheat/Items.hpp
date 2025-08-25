namespace CustomImGui {
	const char* keys[] = {
		"-",
		"Mouse 1",
		"Mouse 2",
		"CN",
		"Mouse 3",
		"Mouse 4",
		"Mouse 5",
		"-",
		"Back",
		"Tab",
		"-",
		"-",
		"CLR",
		"Enter",
		"-",
		"-",
		"Shift",
		"CTL",
		"Menu",
		"Pause",
		"Caps Lock",
		"KAN",
		"-",
		"JUN",
		"FIN",
		"KAN",
		"-",
		"Escape",
		"CON",
		"NCO",
		"ACC",
		"MAD",
		"Space",
		"PGU",
		"PGD",
		"End",
		"Home",
		"Left",
		"Up",
		"Right",
		"Down",
		"SEL",
		"PRI",
		"EXE",
		"PRI",
		"INS",
		"Delete",
		"HEL",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"WIN",
		"WIN",
		"APP",
		"-",
		"SLE",
		"Numpad 0",
		"Numpad 1",
		"Numpad 2",
		"Numpad 3",
		"Numpad 4",
		"Numpad 5",
		"Numpad 6",
		"Numpad 7",
		"Numpad 8",
		"Numpad 9",
		"MUL",
		"ADD",
		"SEP",
		"MIN",
		"Delete",
		"DIV",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"NUM",
		"SCR",
		"EQU",
		"MAS",
		"TOY",
		"OYA",
		"OYA",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"-",
		"Shift",
		"Shift",
		"Ctrl",
		"Ctrl",
		"Alt",
		"Alt"
	};

	struct KeyState {
		ImVec4 background, text;
		bool active = false;
		bool hovered = false;
		float alpha = 0.f;
	};

	struct ButtonState {
		ImVec4 background, text;
	};

	struct PageState {
		ImVec4 background, text, iconColor;
		float smoothSwap, alphaLine, lineSize;
	};

	struct CheckState {
		float fillProgress;
		ImVec4 background = ImVec4(1, 1, 1, 1);
		float  iconAlpha = 0.0f;
		float  iconScale = 0.0f;
	};

	struct BeginState {
		ImVec4 background, text, outline;
		float open, alpha, comboSize = 0.f, shadowOptically;
		bool openedCombo = false, hovered = false;
		float arrowRoll;
	};

	struct SelectState {
		ImVec4 text;
	};

	struct EditState {
		ImVec4 text;
	};

	struct SliderState {
		ImVec4 background, circle, text;
		float position, slow;
	};

	struct PickerState {
		float hue_bar;
		float alpha_bar;
		float circle;
		ImVec2 circle_move;
	};

	ImU32 GetColorU32(const ImVec4& col, float alpha_mul) {
		ImGuiStyle& style = GImGui->Style;
		ImVec4 c = col;
		c.w *= style.Alpha * alpha_mul;
		return ImGui::ColorConvertFloat4ToU32(c);
	}

	ImVec4 ImColorToImVec4(const ImColor& color) {
		return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
	}

	static inline ImDrawFlags FixRectCornerFlags(ImDrawFlags flags) {
		if ((flags & ImDrawFlags_RoundCornersMask_) == 0) {
			flags |= ImDrawFlags_RoundCornersAll;
		}
		return flags;
	}

	void AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left, float rounding = 0, ImDrawFlags flags = 0) {
		auto drawList = ImGui::GetWindowDrawList();
		if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0) {
			return;
		}

		flags = FixRectCornerFlags(flags);
		rounding = ImMin(rounding, ImFabs(p_max.x - p_min.x) * (((flags & ImDrawFlags_RoundCornersTop) == ImDrawFlags_RoundCornersTop) || ((flags & ImDrawFlags_RoundCornersBottom) == ImDrawFlags_RoundCornersBottom) ? 0.5f : 1.0f) - 1.0f);
		rounding = ImMin(rounding, ImFabs(p_max.y - p_min.y) * (((flags & ImDrawFlags_RoundCornersLeft) == ImDrawFlags_RoundCornersLeft) || ((flags & ImDrawFlags_RoundCornersRight) == ImDrawFlags_RoundCornersRight) ? 0.5f : 1.0f) - 1.0f);

		if (rounding > 0.0f) {
			const int size_before = drawList->VtxBuffer.Size;
			drawList->AddRectFilled(p_min, p_max, IM_COL32_WHITE, rounding, flags);
			const int size_after = drawList->VtxBuffer.Size;
			for (int i = size_before; i < size_after; i++) {
				ImDrawVert* vert = drawList->VtxBuffer.Data + i;
				ImVec4 upr_left = ImGui::ColorConvertU32ToFloat4(col_upr_left);
				ImVec4 bot_left = ImGui::ColorConvertU32ToFloat4(col_bot_left);
				ImVec4 up_right = ImGui::ColorConvertU32ToFloat4(col_upr_right);
				ImVec4 bot_right = ImGui::ColorConvertU32ToFloat4(col_bot_right);

				float X = ImClamp((vert->pos.x - p_min.x) / (p_max.x - p_min.x), 0.0f, 1.0f);
				float r1 = upr_left.x + (up_right.x - upr_left.x) * X;
				float r2 = bot_left.x + (bot_right.x - bot_left.x) * X;
				float g1 = upr_left.y + (up_right.y - upr_left.y) * X;
				float g2 = bot_left.y + (bot_right.y - bot_left.y) * X;
				float b1 = upr_left.z + (up_right.z - upr_left.z) * X;
				float b2 = bot_left.z + (bot_right.z - bot_left.z) * X;
				float a1 = upr_left.w + (up_right.w - upr_left.w) * X;
				float a2 = bot_left.w + (bot_right.w - bot_left.w) * X;
				float Y = ImClamp((vert->pos.y - p_min.y) / (p_max.y - p_min.y), 0.0f, 1.0f);
				float r = r1 + (r2 - r1) * Y;
				float g = g1 + (g2 - g1) * Y;
				float b = b1 + (b2 - b1) * Y;
				float a = a1 + (a2 - a1) * Y;
				ImVec4 RGBA(r, g, b, a);
				RGBA = RGBA * ImGui::ColorConvertU32ToFloat4(vert->col);
				vert->col = ImColor(RGBA);
			}
			return;
		}

		const ImVec2 uv = drawList->_Data->TexUvWhitePixel;
		drawList->PrimReserve(6, 4);
		drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx)); drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx + 1)); drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx + 2));
		drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx)); drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx + 2)); drawList->PrimWriteIdx((ImDrawIdx)(drawList->_VtxCurrentIdx + 3));
		drawList->PrimWriteVtx(p_min, uv, col_upr_left);
		drawList->PrimWriteVtx(ImVec2(p_max.x, p_min.y), uv, col_upr_right);
		drawList->PrimWriteVtx(p_max, uv, col_bot_right);
		drawList->PrimWriteVtx(ImVec2(p_min.x, p_max.y), uv, col_bot_left);
	}

	int ImGuiKeyToVirtualKey(ImGuiKey key) {
		switch (key) {
		case ImGuiKey_Tab: return 0x09;  // VK_TAB
		case ImGuiKey_LeftArrow: return 0x25;  // VK_LEFT
		case ImGuiKey_RightArrow: return 0x27;  // VK_RIGHT
		case ImGuiKey_UpArrow: return 0x26;  // VK_UP
		case ImGuiKey_DownArrow: return 0x28;  // VK_DOWN
		case ImGuiKey_PageUp: return 0x21;  // VK_PRIOR
		case ImGuiKey_PageDown: return 0x22;  // VK_NEXT
		case ImGuiKey_Home: return 0x24;  // VK_HOME
		case ImGuiKey_End: return 0x23;  // VK_END
		case ImGuiKey_Insert: return 0x2D;  // VK_INSERT
		case ImGuiKey_Delete: return 0x2E;  // VK_DELETE
		case ImGuiKey_Backspace: return 0x08;  // VK_BACK
		case ImGuiKey_Space: return 0x20;  // VK_SPACE
		case ImGuiKey_Enter: return 0x0D;  // VK_RETURN
		case ImGuiKey_Escape: return 0x1B;  // VK_ESCAPE
		case ImGuiKey_LeftCtrl: return 0xA2;  // VK_LCONTROL
		case ImGuiKey_LeftShift: return 0xA0;  // VK_LSHIFT
		case ImGuiKey_LeftAlt: return 0xA4;  // VK_LMENU
		case ImGuiKey_LeftSuper: return 0x5B;  // VK_LWIN
		case ImGuiKey_RightCtrl: return 0xA3;  // VK_RCONTROL
		case ImGuiKey_RightShift: return 0xA1;  // VK_RSHIFT
		case ImGuiKey_RightAlt: return 0xA5;  // VK_RMENU
		case ImGuiKey_RightSuper: return 0x5C;  // VK_RWIN
		case ImGuiKey_Menu: return 0x5D;  // VK_APPS
		case ImGuiKey_0: return 0x30;  // VK_0
		case ImGuiKey_1: return 0x31;  // VK_1
		case ImGuiKey_2: return 0x32;  // VK_2
		case ImGuiKey_3: return 0x33;  // VK_3
		case ImGuiKey_4: return 0x34;  // VK_4
		case ImGuiKey_5: return 0x35;  // VK_5
		case ImGuiKey_6: return 0x36;  // VK_6
		case ImGuiKey_7: return 0x37;  // VK_7
		case ImGuiKey_8: return 0x38;  // VK_8
		case ImGuiKey_9: return 0x39;  // VK_9
		case ImGuiKey_A: return 0x41;  // VK_A
		case ImGuiKey_B: return 0x42;  // VK_B
		case ImGuiKey_C: return 0x43;  // VK_C
		case ImGuiKey_D: return 0x44;  // VK_D
		case ImGuiKey_E: return 0x45;  // VK_E
		case ImGuiKey_F: return 0x46;  // VK_F
		case ImGuiKey_G: return 0x47;  // VK_G
		case ImGuiKey_H: return 0x48;  // VK_H
		case ImGuiKey_I: return 0x49;  // VK_I
		case ImGuiKey_J: return 0x4A;  // VK_J
		case ImGuiKey_K: return 0x4B;  // VK_K
		case ImGuiKey_L: return 0x4C;  // VK_L
		case ImGuiKey_M: return 0x4D;  // VK_M
		case ImGuiKey_N: return 0x4E;  // VK_N
		case ImGuiKey_O: return 0x4F;  // VK_O
		case ImGuiKey_P: return 0x50;  // VK_P
		case ImGuiKey_Q: return 0x51;  // VK_Q
		case ImGuiKey_R: return 0x52;  // VK_R
		case ImGuiKey_S: return 0x53;  // VK_S
		case ImGuiKey_T: return 0x54;  // VK_T
		case ImGuiKey_U: return 0x55;  // VK_U
		case ImGuiKey_V: return 0x56;  // VK_V
		case ImGuiKey_W: return 0x57;  // VK_W
		case ImGuiKey_X: return 0x58;  // VK_X
		case ImGuiKey_Y: return 0x59;  // VK_Y
		case ImGuiKey_Z: return 0x5A;  // VK_Z
		case ImGuiKey_F1: return 0x70;  // VK_F1
		case ImGuiKey_F2: return 0x71;  // VK_F2
		case ImGuiKey_F3: return 0x72;  // VK_F3
		case ImGuiKey_F4: return 0x73;  // VK_F4
		case ImGuiKey_F5: return 0x74;  // VK_F5
		case ImGuiKey_F6: return 0x75;  // VK_F6
		case ImGuiKey_F7: return 0x76;  // VK_F7
		case ImGuiKey_F8: return 0x77;  // VK_F8
		case ImGuiKey_F9: return 0x78;  // VK_F9
		case ImGuiKey_F10: return 0x79;  // VK_F10
		case ImGuiKey_F11: return 0x7A;  // VK_F11
		case ImGuiKey_F12: return 0x7B;  // VK_F12
		case ImGuiKey_F13: return 0x7C;  // VK_F13
		case ImGuiKey_F14: return 0x7D;  // VK_F14
		case ImGuiKey_F15: return 0x7E;  // VK_F15
		case ImGuiKey_F16: return 0x7F;  // VK_F16
		case ImGuiKey_F17: return 0x80;  // VK_F17
		case ImGuiKey_F18: return 0x81;  // VK_F18
		case ImGuiKey_F19: return 0x82;  // VK_F19
		case ImGuiKey_F20: return 0x83;  // VK_F20
		case ImGuiKey_F21: return 0x84;  // VK_F21
		case ImGuiKey_F22: return 0x85;  // VK_F22
		case ImGuiKey_F23: return 0x86;  // VK_F23
		case ImGuiKey_F24: return 0x87;  // VK_F24
		case ImGuiKey_Apostrophe: return 0xDE;  // VK_OEM_7 (Apostrophe)
		case ImGuiKey_Comma: return 0xBC;  // VK_OEM_COMMA
		case ImGuiKey_Minus: return 0xBD;  // VK_OEM_MINUS
		case ImGuiKey_Period: return 0xBE;  // VK_OEM_PERIOD
		case ImGuiKey_Slash: return 0xBF;  // VK_OEM_2
		case ImGuiKey_Semicolon: return 0xBA;  // VK_OEM_1
		case ImGuiKey_Equal: return 0xBB;  // VK_OEM_PLUS
		case ImGuiKey_LeftBracket: return 0xDB;  // VK_OEM_4
		case ImGuiKey_Backslash: return 0xDC;  // VK_OEM_5
		case ImGuiKey_RightBracket: return 0xDD;  // VK_OEM_6
		case ImGuiKey_GraveAccent: return 0xC0;  // VK_OEM_3
		case ImGuiKey_CapsLock: return 0x14;  // VK_CAPITAL
		case ImGuiKey_ScrollLock: return 0x91;  // VK_SCROLL
		case ImGuiKey_NumLock: return 0x90;  // VK_NUMLOCK
		case ImGuiKey_PrintScreen: return 0x2C;  // VK_SNAPSHOT
		case ImGuiKey_Pause: return 0x13;  // VK_PAUSE
		case ImGuiKey_Keypad0: return 0x60;  // VK_NUMPAD0
		case ImGuiKey_Keypad1: return 0x61;  // VK_NUMPAD1
		case ImGuiKey_Keypad2: return 0x62;  // VK_NUMPAD2
		case ImGuiKey_Keypad3: return 0x63;  // VK_NUMPAD3
		case ImGuiKey_Keypad4: return 0x64;  // VK_NUMPAD4
		case ImGuiKey_Keypad5: return 0x65;  // VK_NUMPAD5
		case ImGuiKey_Keypad6: return 0x66;  // VK_NUMPAD6
		case ImGuiKey_Keypad7: return 0x67;  // VK_NUMPAD7
		case ImGuiKey_Keypad8: return 0x68;  // VK_NUMPAD8
		case ImGuiKey_Keypad9: return 0x69;  // VK_NUMPAD9
		case ImGuiKey_KeypadDecimal: return 0x6E;  // VK_DECIMAL
		case ImGuiKey_KeypadDivide: return 0x6F;  // VK_DIVIDE
		case ImGuiKey_KeypadMultiply: return 0x6A;  // VK_MULTIPLY
		case ImGuiKey_KeypadSubtract: return 0x6D;  // VK_SUBTRACT
		case ImGuiKey_KeypadAdd: return 0x6B;  // VK_ADD
		case ImGuiKey_KeypadEnter: return 0x0D;  // VK_RETURN (Keypad Enter)
		case ImGuiKey_KeypadEqual: return 0x6C;  // VK_SEPARATOR
		case ImGuiKey_AppBack: return 0xA6;  // VK_BROWSER_BACK
		case ImGuiKey_AppForward: return 0xA7;  // VK_BROWSER_FORWARD
		default: return -1;
		}
	}

	ImGuiKey VirtualKeyToImGuiKey(int virtualKey) {
		switch (virtualKey) {
		case 0x09: return ImGuiKey_Tab;  // VK_TAB
		case 0x25: return ImGuiKey_LeftArrow;  // VK_LEFT
		case 0x27: return ImGuiKey_RightArrow;  // VK_RIGHT
		case 0x26: return ImGuiKey_UpArrow;  // VK_UP
		case 0x28: return ImGuiKey_DownArrow;  // VK_DOWN
		case 0x21: return ImGuiKey_PageUp;  // VK_PRIOR
		case 0x22: return ImGuiKey_PageDown;  // VK_NEXT
		case 0x24: return ImGuiKey_Home;  // VK_HOME
		case 0x23: return ImGuiKey_End;  // VK_END
		case 0x2D: return ImGuiKey_Insert;  // VK_INSERT
		case 0x2E: return ImGuiKey_Delete;  // VK_DELETE
		case 0x08: return ImGuiKey_Backspace;  // VK_BACK
		case 0x20: return ImGuiKey_Space;  // VK_SPACE
		case 0x1B: return ImGuiKey_Escape;  // VK_ESCAPE
		case 0xA2: return ImGuiKey_LeftCtrl;  // VK_LCONTROL
		case 0xA0: return ImGuiKey_LeftShift;  // VK_LSHIFT
		case 0xA4: return ImGuiKey_LeftAlt;  // VK_LMENU
		case 0x5B: return ImGuiKey_LeftSuper;  // VK_LWIN
		case 0xA3: return ImGuiKey_RightCtrl;  // VK_RCONTROL
		case 0xA1: return ImGuiKey_RightShift;  // VK_RSHIFT
		case 0xA5: return ImGuiKey_RightAlt;  // VK_RMENU
		case 0x5C: return ImGuiKey_RightSuper;  // VK_RWIN
		case 0x5D: return ImGuiKey_Menu;  // VK_APPS
		case 0x30: return ImGuiKey_0;  // VK_0
		case 0x31: return ImGuiKey_1;  // VK_1
		case 0x32: return ImGuiKey_2;  // VK_2
		case 0x33: return ImGuiKey_3;  // VK_3
		case 0x34: return ImGuiKey_4;  // VK_4
		case 0x35: return ImGuiKey_5;  // VK_5
		case 0x36: return ImGuiKey_6;  // VK_6
		case 0x37: return ImGuiKey_7;  // VK_7
		case 0x38: return ImGuiKey_8;  // VK_8
		case 0x39: return ImGuiKey_9;  // VK_9
		case 0x41: return ImGuiKey_A;  // VK_A
		case 0x42: return ImGuiKey_B;  // VK_B
		case 0x43: return ImGuiKey_C;  // VK_C
		case 0x44: return ImGuiKey_D;  // VK_D
		case 0x45: return ImGuiKey_E;  // VK_E
		case 0x46: return ImGuiKey_F;  // VK_F
		case 0x47: return ImGuiKey_G;  // VK_G
		case 0x48: return ImGuiKey_H;  // VK_H
		case 0x49: return ImGuiKey_I;  // VK_I
		case 0x4A: return ImGuiKey_J;  // VK_J
		case 0x4B: return ImGuiKey_K;  // VK_K
		case 0x4C: return ImGuiKey_L;  // VK_L
		case 0x4D: return ImGuiKey_M;  // VK_M
		case 0x4E: return ImGuiKey_N;  // VK_N
		case 0x4F: return ImGuiKey_O;  // VK_O
		case 0x50: return ImGuiKey_P;  // VK_P
		case 0x51: return ImGuiKey_Q;  // VK_Q
		case 0x52: return ImGuiKey_R;  // VK_R
		case 0x53: return ImGuiKey_S;  // VK_S
		case 0x54: return ImGuiKey_T;  // VK_T
		case 0x55: return ImGuiKey_U;  // VK_U
		case 0x56: return ImGuiKey_V;  // VK_V
		case 0x57: return ImGuiKey_W;  // VK_W
		case 0x58: return ImGuiKey_X;  // VK_X
		case 0x59: return ImGuiKey_Y;  // VK_Y
		case 0x5A: return ImGuiKey_Z;  // VK_Z
		case 0x70: return ImGuiKey_F1;  // VK_F1
		case 0x71: return ImGuiKey_F2;  // VK_F2
		case 0x72: return ImGuiKey_F3;  // VK_F3
		case 0x73: return ImGuiKey_F4;  // VK_F4
		case 0x74: return ImGuiKey_F5;  // VK_F5
		case 0x75: return ImGuiKey_F6;  // VK_F6
		case 0x76: return ImGuiKey_F7;  // VK_F7
		case 0x77: return ImGuiKey_F8;  // VK_F8
		case 0x78: return ImGuiKey_F9;  // VK_F9
		case 0x79: return ImGuiKey_F10;  // VK_F10
		case 0x7A: return ImGuiKey_F11;  // VK_F11
		case 0x7B: return ImGuiKey_F12;  // VK_F12
		case 0x7C: return ImGuiKey_F13;  // VK_F13
		case 0x7D: return ImGuiKey_F14;  // VK_F14
		case 0x7E: return ImGuiKey_F15;  // VK_F15
		case 0x7F: return ImGuiKey_F16;  // VK_F16
		case 0x80: return ImGuiKey_F17;  // VK_F17
		case 0x81: return ImGuiKey_F18;  // VK_F18
		case 0x82: return ImGuiKey_F19;  // VK_F19
		case 0x83: return ImGuiKey_F20;  // VK_F20
		case 0x84: return ImGuiKey_F21;  // VK_F21
		case 0x85: return ImGuiKey_F22;  // VK_F22
		case 0x86: return ImGuiKey_F23;  // VK_F23
		case 0x87: return ImGuiKey_F24;  // VK_F24
		case 0xDE: return ImGuiKey_Apostrophe;  // VK_OEM_7 (Apostrophe)
		case 0xBC: return ImGuiKey_Comma;  // VK_OEM_COMMA
		case 0xBD: return ImGuiKey_Minus;  // VK_OEM_MINUS
		case 0xBE: return ImGuiKey_Period;  // VK_OEM_PERIOD
		case 0xBF: return ImGuiKey_Slash;  // VK_OEM_2
		case 0xBA: return ImGuiKey_Semicolon;  // VK_OEM_1
		case 0xBB: return ImGuiKey_Equal;  // VK_OEM_PLUS
		case 0xDB: return ImGuiKey_LeftBracket;  // VK_OEM_4
		case 0xDC: return ImGuiKey_Backslash;  // VK_OEM_5
		case 0xDD: return ImGuiKey_RightBracket;  // VK_OEM_6
		case 0xC0: return ImGuiKey_GraveAccent;  // VK_OEM_3
		case 0x14: return ImGuiKey_CapsLock;  // VK_CAPITAL
		case 0x91: return ImGuiKey_ScrollLock;  // VK_SCROLL
		case 0x90: return ImGuiKey_NumLock;  // VK_NUMLOCK
		case 0x2C: return ImGuiKey_PrintScreen;  // VK_SNAPSHOT
		case 0x13: return ImGuiKey_Pause;  // VK_PAUSE
		case 0x60: return ImGuiKey_Keypad0;  // VK_NUMPAD0
		case 0x61: return ImGuiKey_Keypad1;  // VK_NUMPAD1
		case 0x62: return ImGuiKey_Keypad2;  // VK_NUMPAD2
		case 0x63: return ImGuiKey_Keypad3;  // VK_NUMPAD3
		case 0x64: return ImGuiKey_Keypad4;  // VK_NUMPAD4
		case 0x65: return ImGuiKey_Keypad5;  // VK_NUMPAD5
		case 0x66: return ImGuiKey_Keypad6;  // VK_NUMPAD6
		case 0x67: return ImGuiKey_Keypad7;  // VK_NUMPAD7
		case 0x68: return ImGuiKey_Keypad8;  // VK_NUMPAD8
		case 0x69: return ImGuiKey_Keypad9;  // VK_NUMPAD9
		case 0x6E: return ImGuiKey_KeypadDecimal;  // VK_DECIMAL
		case 0x6F: return ImGuiKey_KeypadDivide;  // VK_DIVIDE
		case 0x6A: return ImGuiKey_KeypadMultiply;  // VK_MULTIPLY
		case 0x6D: return ImGuiKey_KeypadSubtract;  // VK_SUBTRACT
		case 0x6B: return ImGuiKey_KeypadAdd;  // VK_ADD
		case 0x6C: return ImGuiKey_KeypadEqual;  // VK_SEPARATOR
		case 0xA6: return ImGuiKey_AppBack;  // VK_BROWSER_BACK
		case 0xA7: return ImGuiKey_AppForward;  // VK_BROWSER_FORWARD
		default: return ImGuiKey_None;  // Unknown virtual key
		}
	}

	bool Keybind(const char* label, int* key, int* mode) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		std::string modifiedLabel = label;
		modifiedLabel += "2";
		const ImGuiID id2 = window->GetID(modifiedLabel.c_str());
		const float width = (ImGui::GetContentRegionMax().x - style.WindowPadding.x);

		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 pos2 = window->DC.CursorPos;

		pos.y += labelSize.y + style.ItemSpacing.y + 7;
		window->DC.CursorPos = pos;

		const ImRect rect(pos, pos + ImVec2(width, 32));
		ImGui::ItemSize(rect);
		if (!ImGui::ItemAdd(rect, id)) return false;

		char bufDisplay[64] = "None";
		bool valueChanged = false;
		int k = *key;

		if (*key != 0 && g.ActiveId != id) {
			if (*key == 1 || *key == 2 || *key == 3 || *key == 4 || *key == 5 || *key == 6) {
				std::string active_key = "";
				active_key += keys[*key];
				strcpy_s(bufDisplay, active_key.c_str());
			}
			else {
				strcpy_s(bufDisplay, ImGui::GetKeyName(VirtualKeyToImGuiKey(k)));
			}
		}
		else if (g.ActiveId == id) {
			strcpy_s(bufDisplay, "...");
		}

		const ImVec2 bufDisplaySize = ImGui::CalcTextSize(bufDisplay, NULL, true);
		ImRect clickable(rect.Min, rect.Max);
		ImRect clickable2(rect.Min - ImVec2(0, 30), rect.Max + ImVec2(0, 10));

		bool hovered = ImGui::ItemHoverable(clickable, id, NULL);
		bool hovered2 = ImGui::ItemHoverable(clickable2, id2, NULL);

		if (hovered || hovered2) {
			window->DrawList->AddText(pos2, Menu.TextHoverColor, label);
		}
		else {
			window->DrawList->AddText(pos2, Menu.TextColor, label);
		}

		static std::map<ImGuiID, KeyState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, KeyState() });
			itAnim = anim.find(id);
		}

		itAnim->second.text = ImLerp(itAnim->second.text, g.ActiveId == id ? Menu.TextActiveColor : hovered ? Menu.TextHoverColor : Menu.TextColor, ImGui::GetIO().DeltaTime * 6.f);
		window->DrawList->AddRectFilled(clickable.Min, clickable.Max, Menu.ElementsColor, Menu.ElementsRounding);
		ImGui::PushStyleColor(ImGuiCol_Text, ImColorToImVec4(Menu.TextColor));
		ImGui::PushFont(Menu.FontAwesome);
		ImGui::RenderTextClipped(clickable.Min + ImVec2(10, (32 - ImGui::CalcTextSize(ICON_FA_KEYBOARD).y) / 2), clickable.Max, ICON_FA_KEYBOARD, NULL, NULL, ImVec2(0.0f, 0.0f));
		ImGui::PopFont();
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, ImColorToImVec4(Menu.TextActiveColor));
		ImGui::RenderTextClipped(clickable.Min + ImVec2(40, (32 - bufDisplaySize.y) / 2), clickable.Max, bufDisplay, NULL, &bufDisplaySize, ImVec2(0.0f, 0.0f));
		ImGui::PopStyleColor();

		if (hovered && io.MouseClicked[0]) {
			if (g.ActiveId != id) {
				io.ClearInputKeys();
				io.ClearInputMouse();
				*key = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {
			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io.MouseDown[i]) {
					switch (i) {
					case 0:
						k = 0x01;
						break;
					case 1:
						k = 0x02;
						break;
					case 2:
						k = 0x04;
						break;
					case 3:
						k = 0x05;
						break;
					case 4:
						k = 0x06;
						break;
					}
					valueChanged = true;
					ImGui::ClearActiveID();
				}
			}
			if (!valueChanged) {
				for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) {
					if (ImGui::IsKeyDown(key)) {
						k = ImGuiKeyToVirtualKey(key);
						valueChanged = true;
						ImGui::ClearActiveID();
					}
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
				*key = 0;
				ImGui::ClearActiveID();
			}
			else {
				*key = k;
			}
		}
		return valueChanged;
	}

	bool ChildEx(const char* name, ImGuiID id, const ImVec2& sizeArg, bool cap, ImGuiWindowFlags flags) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* parentWindow = g.CurrentWindow;

		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
		flags |= (parentWindow->Flags & ImGuiWindowFlags_NoMove);

		const ImVec2 contentAvail = ImGui::GetContentRegionAvail();
		ImVec2 size = ImTrunc(sizeArg);
		const int autoFitAxises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
		if (size.x <= 0.0f) size.x = ImMax(contentAvail.x + size.x, 4.0f);
		if (size.y <= 0.0f) size.y = ImMax(contentAvail.y + size.y, 4.0f);

		ImGui::SetNextWindowPos(ImVec2(parentWindow->DC.CursorPos + ImVec2(0, cap ? 25 : 0)));
		ImGui::SetNextWindowSize(size - ImVec2(0, cap ? 25 : 0));
		ImGui::GetWindowDrawList()->AddRectFilled(parentWindow->DC.CursorPos + ImVec2(0, cap ? 25 : 0), parentWindow->DC.CursorPos + sizeArg, Menu.ChildColor, Menu.ChildRounding, cap ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersAll);
		ImVec2 minPos = parentWindow->DC.CursorPos + ImVec2(0, cap ? 25 : 0);
		ImVec2 maxPos = parentWindow->DC.CursorPos + sizeArg;

		if (cap) {
			ImVec2 capMinPos = parentWindow->DC.CursorPos;
			ImVec2 capMaxPos = parentWindow->DC.CursorPos + ImVec2(sizeArg.x, 25);
			ImGui::GetWindowDrawList()->AddRectFilled(capMinPos, capMaxPos, Menu.ChildCapColor, 0, ImDrawFlags_RoundCornersTop);
			ImGui::GetWindowDrawList()->AddText(ImVec2(capMinPos.x + 10, capMinPos.y + (25 - ImGui::CalcTextSize(name).y) / 2), Menu.PageTextColor, name);
		}

		const char* tempWindowName;
		if (name) {
			ImFormatStringToTempBuffer(&tempWindowName, NULL, "%s/%s_%08X", parentWindow->Name, name, id);
		}
		else {
			ImFormatStringToTempBuffer(&tempWindowName, NULL, "%s/%08X", parentWindow->Name, id);
		}

		const float backupBorderSize = g.Style.ChildBorderSize;
		bool ret = ImGui::Begin(tempWindowName, NULL, flags | ImGuiWindowFlags_NoBackground);
		ImGuiWindow* childWindow = g.CurrentWindow;
		childWindow->ChildId = id;

		if (childWindow->BeginCount == 1) {
			parentWindow->DC.CursorPos = childWindow->Pos;
		}

		const ImGuiID tempIdForActivation = ImHashStr("##Child", 0, id);
		if (g.ActiveId == tempIdForActivation) ImGui::ClearActiveID();
		if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (childWindow->DC.NavLayersActiveMask != 0 || childWindow->DC.NavWindowHasScrollY)) {
			ImGui::FocusWindow(childWindow);
			ImGui::NavInitWindow(childWindow, false);
			ImGui::SetActiveID(tempIdForActivation, childWindow);
			g.ActiveIdSource = g.NavInputSource;
		}
		return ret;
	}

	bool ChildEx2(const char* name, ImGuiID id, const ImVec2& sizeArg, ImGuiWindowFlags flags) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* parentWindow = g.CurrentWindow;

		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
		flags |= (parentWindow->Flags & ImGuiWindowFlags_NoMove);

		const ImVec2 contentAvail = ImGui::GetContentRegionAvail();
		ImVec2 size = ImTrunc(sizeArg);
		const int autoFitAxises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
		if (size.x <= 0.0f) size.x = ImMax(contentAvail.x + size.x, 4.0f);
		if (size.y <= 0.0f) size.y = ImMax(contentAvail.y + size.y, 4.0f);

		ImGui::SetNextWindowPos(ImVec2(parentWindow->DC.CursorPos));
		ImGui::SetNextWindowSize(size);

		const char* tempWindowName;
		if (name) {
			ImFormatStringToTempBuffer(&tempWindowName, NULL, "%s/%s_%08X", parentWindow->Name, name, id);
		}
		else {
			ImFormatStringToTempBuffer(&tempWindowName, NULL, "%s/%08X", parentWindow->Name, id);
		}

		const float backupBorderSize = g.Style.ChildBorderSize;
		bool ret = ImGui::Begin(tempWindowName, NULL, flags | ImGuiWindowFlags_NoBackground);
		ImGuiWindow* childWindow = g.CurrentWindow;
		childWindow->ChildId = id;

		if (childWindow->BeginCount == 1) {
			parentWindow->DC.CursorPos = childWindow->Pos;
		}

		const ImGuiID tempIdForActivation = ImHashStr("##Child", 0, id);
		if (g.ActiveId == tempIdForActivation) ImGui::ClearActiveID();
		if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (childWindow->DC.NavLayersActiveMask != 0 || childWindow->DC.NavWindowHasScrollY)) {
			ImGui::FocusWindow(childWindow);
			ImGui::NavInitWindow(childWindow, false);
			ImGui::SetActiveID(tempIdForActivation, childWindow);
			g.ActiveIdSource = g.NavInputSource;
		}
		return ret;
	}

	bool Child(const char* strId, const ImVec2& sizeArg, bool cap, ImGuiWindowFlags extraFlags = 0) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13, 13));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(13, 13));
		std::string modifiedLabel = strId;
		modifiedLabel += "Child";
		return ChildEx(strId, window->GetID(modifiedLabel.c_str()), sizeArg, cap, extraFlags | ImGuiWindowFlags_AlwaysUseWindowPadding);
	}

	bool Child2(const char* strId, const ImVec2& sizeArg, ImGuiWindowFlags extraFlags = 0) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13, 13));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(13, 13));
		std::string modifiedLabel = strId;
		modifiedLabel += "Child";
		return ChildEx2(strId, window->GetID(modifiedLabel.c_str()), sizeArg, extraFlags | ImGuiWindowFlags_AlwaysUseWindowPadding);
	}

	void EndChild() {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGui::PopStyleVar(2);
		IM_ASSERT(g.WithinEndChild == false);
		IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);
		g.WithinEndChildID = window->ID;

		if (window->BeginCount > 1) {
			ImGui::End();
		}
		else {
			ImVec2 sz = window->Size;
			ImGui::End();
			ImGuiWindow* parentWindow = g.CurrentWindow;
			ImRect bb(parentWindow->DC.CursorPos, parentWindow->DC.CursorPos + sz);
			ImGui::ItemSize(sz);
			if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavWindowHasScrollY) && !(window->Flags & ImGuiWindowFlags_NavFlattened)) {
				ImGui::ItemAdd(bb, window->ChildId);
			}
			else {
				ImGui::ItemAdd(bb, 0);
				if (window->Flags & ImGuiWindowFlags_NavFlattened) {
					parentWindow->DC.NavLayersActiveMaskNext |= window->DC.NavLayersActiveMaskNext;
				}
			}

			if (g.HoveredWindow == window) {
				g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
			}
		}
		g.WithinEndChildID = 0;
		g.LogLinePosY = -FLT_MAX;
	}

	void BeginGroup() {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		g.GroupStack.resize(g.GroupStack.Size + 1);
		ImGuiGroupData& groupData = g.GroupStack.back();
		groupData.WindowID = window->ID;
		groupData.BackupCursorPos = window->DC.CursorPos;
		groupData.BackupCursorMaxPos = window->DC.CursorMaxPos;
		groupData.BackupIndent = window->DC.Indent;
		groupData.BackupGroupOffset = window->DC.GroupOffset;
		groupData.BackupCurrLineSize = window->DC.CurrLineSize;
		groupData.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
		groupData.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
		groupData.BackupHoveredIdIsAlive = g.HoveredId != 0;
		groupData.BackupDeactivatedIdIsAlive = g.DeactivatedItemData.IsAlive;
		groupData.EmitItem = true;
		window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
		window->DC.Indent = window->DC.GroupOffset;
		window->DC.CursorMaxPos = window->DC.CursorPos;
		window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
		if (g.LogEnabled) {
			g.LogLinePosY = -FLT_MAX;
		}
	}

	void EndGroup() {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		IM_ASSERT(g.groupStack.Size > 0);

		ImGuiGroupData& groupData = g.GroupStack.back();
		IM_ASSERT(groupData.windowID == window->ID);

		if (window->DC.IsSetPos) {
			ImGui::ErrorCheckUsingSetCursorPosToExtendParentBoundaries();
		}

		ImRect groupBb(groupData.BackupCursorPos, ImMax(window->DC.CursorMaxPos, groupData.BackupCursorPos));
		window->DC.CursorPos = groupData.BackupCursorPos;
		window->DC.CursorMaxPos = ImMax(groupData.BackupCursorMaxPos, window->DC.CursorMaxPos);
		window->DC.Indent = groupData.BackupIndent;
		window->DC.GroupOffset = groupData.BackupGroupOffset;
		window->DC.CurrLineSize = groupData.BackupCurrLineSize;
		window->DC.CurrLineTextBaseOffset = groupData.BackupCurrLineTextBaseOffset;

		if (g.LogEnabled) {
			g.LogLinePosY = -FLT_MAX;
		}

		if (!groupData.EmitItem) {
			g.GroupStack.pop_back();
			return;
		}

		window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, groupData.BackupCurrLineTextBaseOffset);
		ImGui::ItemSize(groupBb.GetSize());
		ImGui::ItemAdd(groupBb, 0, NULL, ImGuiItemFlags_NoTabStop);

		const bool groupContainsCurrActiveId = (groupData.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
		const bool groupContainsPrevActiveId = (groupData.BackupDeactivatedIdIsAlive == false) && (g.DeactivatedItemData.IsAlive == true);

		if (groupContainsCurrActiveId) {
			g.LastItemData.ID = g.ActiveId;
		}
		else if (groupContainsPrevActiveId) {
			g.LastItemData.ID = g.ActiveIdPreviousFrame;
		}

		g.LastItemData.Rect = groupBb;
		const bool groupContainsCurrHoveredId = (groupData.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;

		if (groupContainsCurrHoveredId) {
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
		}

		if (groupContainsCurrActiveId && g.ActiveIdHasBeenEditedThisFrame) {
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;
		}

		g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
		if (groupContainsPrevActiveId && g.ActiveId != g.ActiveIdPreviousFrame) {
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;
		}

		g.GroupStack.pop_back();
	}

	bool Button(const char* label, const ImVec2& sizeArg) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true), pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize(sizeArg, labelSize.x, labelSize.y);
		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, 0.f);

		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held, pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);
		static std::map<ImGuiID, ButtonState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, ButtonState() });
			itAnim = anim.find(id);
		}

		itAnim->second.background = ImLerp(itAnim->second.background, ImGui::IsItemActive() || hovered ? Menu.HeaderColor : Menu.ElementsColor, g.IO.DeltaTime * 6.f);
		itAnim->second.text = ImLerp(itAnim->second.text, ImGui::IsItemActive() || hovered ? Menu.TextActiveColor : Menu.PageTextColor, g.IO.DeltaTime * 6.f);
		ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(itAnim->second.background), Menu.PageRounding);
		ImGui::PushClipRect(bb.Min, bb.Max, true);
		ImGui::PushFont(Menu.Inter);
		ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (sizeArg.x - ImGui::CalcTextSize(label).x) / 2, bb.Max.y - ImGui::CalcTextSize(label).y - (size.y - ImGui::CalcTextSize(label).y) / 2), ImGui::GetColorU32(itAnim->second.text), label);
		ImGui::PopFont();
		ImGui::PopClipRect();
		return pressed;
	}

	bool Page(bool selected, const char* icon, const char* label, const ImVec2& sizeArg) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 iconSize = ImGui::CalcTextSize(icon, NULL, true);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const float spacing = 12.0f;
		const float iconOffset = 4.0f;
		ImVec2 size = ImGui::CalcItemSize(sizeArg, iconSize.x + labelSize.x + spacing + style.FramePadding.x * 3, iconSize.y);
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held, pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);
		static std::map<ImGuiID, PageState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, PageState() });
			itAnim = anim.find(id);
		}

		ImColor backgroundColor = selected ? Menu.PageActiveColor : ImColor(0, 0, 0);
		ImColor backgroundColorIcon = selected ? Menu.PageActiveIconColor : ImColor(207, 225, 255);
		ImColor iconColor = selected ? ImColor(171, 170, 186) : Menu.PageTextColor;
		ImColor textColor = selected ? Menu.TextActiveColor : hovered ? ImColor(255, 255, 255) : Menu.PageTextColor;

		itAnim->second.background = ImLerp(itAnim->second.background, backgroundColor, g.IO.DeltaTime * 6.f);
		itAnim->second.text = ImLerp(itAnim->second.text, textColor, g.IO.DeltaTime * 14.f);
		itAnim->second.iconColor = ImLerp(itAnim->second.background, iconColor, g.IO.DeltaTime * 6.f);

		static std::map<ImGuiID, float> animationProgressMap;
		float& animationProgress = animationProgressMap[id];
		if (selected) {
			animationProgress += g.IO.DeltaTime * 3.0f;
			if (animationProgress > 1.0f) {
				animationProgress = 1.0f;
			}
		}
		else {
			animationProgress = 0.0f;
		}
		float animatedHeight = (bb.Max.y - bb.Min.y) * animationProgress;

		if (selected) {
			ImGui::GetWindowDrawList()->AddRectFilled(bb.Min - ImVec2(20, 0), bb.Max + ImVec2(30, 0), ImGui::GetColorU32(itAnim->second.background));
			ImVec2 lineStart = ImVec2(bb.Min.x, bb.Min.y);
			ImVec2 lineEnd = ImVec2(bb.Min.x + 3.0f, bb.Min.y + animatedHeight);
			ImGui::GetWindowDrawList()->AddRectFilled(lineStart - ImVec2(20, 0), lineEnd - ImVec2(10, 0), Menu.MainColor, 1.0f);
		}

		ImGui::PushClipRect(bb.Min, bb.Max, true);
		ImGui::PushFont(Menu.FontAwesome);
		ImVec2 iconPos = ImVec2(bb.Min.x + style.FramePadding.x + iconOffset, bb.Min.y + (size.y - iconSize.y) / 2);
		iconPos += ImVec2(-5, 4);

		if (selected) {
			ImGui::GetWindowDrawList()->AddText(iconPos, iconColor, icon);
		}
		else {
			ImGui::GetWindowDrawList()->AddText(iconPos, iconColor, icon);
		}

		ImGui::PopFont();
		ImVec2 labelPos = ImVec2(iconPos.x + iconSize.x + spacing, bb.Min.y + (size.y - labelSize.y) / 2);
		labelPos += ImVec2(5, 0);
		if (selected) {
			ImGui::GetWindowDrawList()->AddText(labelPos, ImGui::GetColorU32(itAnim->second.text), label);
		}
		else {
			ImGui::GetWindowDrawList()->AddText(labelPos, ImGui::GetColorU32(itAnim->second.text), label);
		}

		ImGui::PopClipRect();
		return pressed;
	}

	bool SubTab(bool selected, const char* label, const ImVec2& sizeArg) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		std::string modifiedLabel = label;
		modifiedLabel += "Tab";

		ImGuiContext& g = *GImGui;
		const auto& style = g.Style;
		const ImGuiID id = window->GetID(modifiedLabel.c_str());
		const ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);
		const float spacing = 12.0f;
		ImVec2 size = ImGui::CalcItemSize(sizeArg, labelSize.x + spacing + style.FramePadding.x * 3, labelSize.y);
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		static std::unordered_map<ImGuiID, PageState> anim;
		auto& state = anim[id];

		ImColor backgroundColor = selected ? Menu.PageActiveColor : ImColor(0, 0, 0);
		ImColor textColor = selected ? Menu.TextActiveColor : (hovered ? ImColor(255, 255, 255) : Menu.PageTextColor);

		state.background = ImLerp(state.background, backgroundColor, g.IO.DeltaTime * 6.0f);
		state.text = ImLerp(state.text, textColor, g.IO.DeltaTime * 14.0f);

		ImGui::PushClipRect(bb.Min, bb.Max, true);
		ImVec2 labelPos = bb.Min + (size - labelSize) * 0.5f;
		window->DrawList->AddText(labelPos, ImGui::GetColorU32(state.text), label);
		ImGui::PopClipRect();
		return pressed;
	}

	void Separator(const char* label) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return;
		}

		const ImGuiStyle& style = ImGui::GetStyle();
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const float separatorHeight = labelSize.y + style.FramePadding.y * 2;
		const float separatorWidth = labelSize.x + style.FramePadding.x * 2;
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(separatorWidth, separatorHeight));
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, ImGui::GetID(label))) {
			return;
		}

		ImVec2 textPos = ImVec2(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y);
		ImGui::GetWindowDrawList()->AddText(textPos, Menu.PageTextColor, label);
	}

	bool AvatarWithLabel(const char* label, const ImVec2& avatarSize, const ImVec4& avatarColor) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		static float currentScale = 1.0f;
		static ImVec4 currentColor = avatarColor;
		const float targetScale = 1.1f;
		const float animationSpeed = 0.1f;
		std::string modifiedLabel;
		size_t labelLen = strlen(label);
		if (labelLen > 6) {
			std::string tempLabel(label, 8);
			modifiedLabel = tempLabel.substr(0, 6) + "**";
		}
		else {
			modifiedLabel = label;
		}

		ImVec2 labelSize = ImGui::CalcTextSize(modifiedLabel.c_str());
		float spacing = style.ItemSpacing.x * 0.5f;

		ImVec2 size = ImGui::CalcItemSize(avatarSize, labelSize.x + spacing + avatarSize.x, avatarSize.y);
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id)) {
			return false;
		}

		bool hovered, held;
		ImGui::ButtonBehavior(bb, id, &hovered, &held);
		float targetScaleFactor = hovered ? targetScale : 1.0f;
		currentScale += (targetScaleFactor - currentScale) * animationSpeed;
		ImVec4 targetColor = hovered ? ImVec4(avatarColor.x * 1.3f, avatarColor.y * 1.3f, avatarColor.z * 1.3f, avatarColor.w) : avatarColor;
		currentColor.x += (targetColor.x - currentColor.x) * animationSpeed;
		currentColor.y += (targetColor.y - currentColor.y) * animationSpeed;
		currentColor.z += (targetColor.z - currentColor.z) * animationSpeed;
		currentColor.w += (targetColor.w - currentColor.w) * animationSpeed;

		ImVec2 scaledAvatarSize = ImVec2(avatarSize.x * currentScale, avatarSize.y * currentScale);
		ImVec2 labelPos = ImVec2(bb.Min.x, bb.Min.y + (avatarSize.y - labelSize.y) * 0.5f);
		ImVec2 circleCenter = ImVec2(bb.Min.x + labelSize.x + spacing + scaledAvatarSize.x * 0.5f, bb.Min.y + scaledAvatarSize.y * 0.5f);
		float circleRadius = scaledAvatarSize.x * 0.5f;
		ImVec2 expandedMin = bb.Min;
		ImVec2 expandedMax = bb.Max;
		expandedMin.x -= 10.0f;
		expandedMax.x += 60.0f;
		if (hovered) {
			ImGui::GetWindowDrawList()->AddCircleFilled(circleCenter, circleRadius + 5.0f, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.2f)));
		}

		window->DrawList->AddRectFilled(expandedMin, expandedMax, Menu.PageActiveColor, 6.0f);
		ImU32 colorTop = ImGui::GetColorU32(ImVec4(0.5f * currentColor.x, 0.5f * currentColor.y, 0.5f * currentColor.z, currentColor.w));
		ImU32 colorBottom = ImGui::GetColorU32(currentColor);

		ImGui::GetWindowDrawList()->AddCircleFilled(circleCenter, circleRadius, colorBottom);
		ImGui::GetWindowDrawList()->AddCircle(circleCenter, circleRadius, colorTop, 64, 2.0f);

		std::string initials = (strlen(label) >= 1) ? std::string(label, 1) : label;
		ImVec2 textSize = ImGui::CalcTextSize(initials.c_str());
		ImVec2 textPos = ImVec2(circleCenter.x - textSize.x * 0.5f, circleCenter.y - textSize.y * 0.5f);
		ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), initials.c_str());
		ImGui::GetWindowDrawList()->AddText(labelPos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), modifiedLabel.c_str());
		return ImGui::IsItemClicked();
	}

	bool Checkbox(const char* label, bool* v) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (!window || window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);

		const float boxSize = 18.0f;
		const float padding = 2.0f;
		ImVec2 pos = window->DC.CursorPos;
		ImRect checkboxBB(pos, pos + ImVec2(boxSize, boxSize));
		ImRect textBB(pos + ImVec2(boxSize + style.ItemInnerSpacing.x + 3, 0),
			pos + ImVec2(boxSize + style.ItemInnerSpacing.x + labelSize.x, boxSize));

		ImGui::ItemSize(textBB, style.FramePadding.y);
		if (!ImGui::ItemAdd(checkboxBB, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(checkboxBB, id, &hovered, &held);

		static std::map<ImGuiID, CheckState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, CheckState{} });
			itAnim = anim.find(id);
		}
		CheckState& st = itAnim->second;

		ImVec4 targetColor = *v? Menu.CheckboxMarkColor : (hovered ? Menu.ElementsHoverColor : Menu.ElementsColor);
		st.background = ImLerp(st.background, targetColor, g.IO.DeltaTime * 8.f);

		float targetFill = *v ? 1.0f : 0.0f;
		st.fillProgress = ImLerp(st.fillProgress, targetFill, g.IO.DeltaTime * 8.f);

		float iconTarget = *v ? 1.0f : 0.0f;
		st.iconAlpha = ImLerp(st.iconAlpha, iconTarget, g.IO.DeltaTime * 8.f);

		st.iconScale = ImLerp(st.iconScale, iconTarget, g.IO.DeltaTime * 10.f);

		if (pressed) {
			*v = !(*v);
			ImGui::MarkItemEdited(id);
		}

		if (st.fillProgress > 0.01f) {
			ImVec2 center = (checkboxBB.Min + checkboxBB.Max) * 0.5f;
			ImVec2 margin = ImVec2(5.0f, 5.0f);
			ImVec2 baseSize = (checkboxBB.Max - checkboxBB.Min) - ImVec2(padding, padding) - margin;
			ImVec2 fillExtent = ImLerp(ImVec2(0, 0), baseSize, st.fillProgress);
			ImVec2 half = fillExtent * 0.5f;
			ImU32 fillCol = ImGui::GetColorU32(st.background);
			window->DrawList->AddRectFilled(center - half, center + half, fillCol, 4.0f);
		}

		if (st.iconAlpha > 0.001f) {
			float alphaF = st.iconAlpha * 255.f;
			int alphaI = (int)alphaF;
			if (alphaI < 0)   alphaI = 0;
			if (alphaI > 255) alphaI = 255;
			float scale = 0.5f + 0.5f * st.iconScale;
			ImU32 iconColor = IM_COL32(255, 255, 255, alphaI);
			const char* iconChar = ICON_FA_CHECK;
			float oldFontSize = Menu.FontAwesome->FontSize;
			Menu.FontAwesome->FontSize = oldFontSize * scale;
			ImGui::PushFont(Menu.FontAwesome);
			ImVec2 iconSize = ImGui::CalcTextSize(iconChar);
			ImVec2 center = (checkboxBB.Min + checkboxBB.Max) * 0.5f;
			ImVec2 iconPos(center.x - iconSize.x * 0.5f, center.y - iconSize.y * 0.5f);
			window->DrawList->AddText(Menu.FontAwesome, Menu.FontAwesome->FontSize, iconPos, iconColor, iconChar);
			ImGui::PopFont();
			Menu.FontAwesome->FontSize = oldFontSize;
		}

		ImU32 textColor = *v ? Menu.TextActiveColor : Menu.TextColor;
		window->DrawList->AddText(textBB.Min, textColor, label);
		if (*v) {
			window->DrawList->AddRect(checkboxBB.Min, checkboxBB.Max, Menu.MainColor, 4.0f, ImDrawFlags_RoundCornersAll, 2.0f);
		}
		else {
			ImU32 borderColor = hovered ? Menu.MainColor : Menu.HeaderColor;
			window->DrawList->AddRect(checkboxBB.Min, checkboxBB.Max, borderColor, 4.0f, ImDrawFlags_RoundCornersAll, 2.0f);
		}
		return pressed;
	}

	bool RadialCheckbox(const char* label, bool* v)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (!window || window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);
		const float boxSize = 22.0f;
		const float padding = 2.0f;
		ImVec2 pos = window->DC.CursorPos;
		ImRect checkboxBB(pos, pos + ImVec2(boxSize, boxSize));
		ImRect textBB(pos + ImVec2(boxSize + style.ItemInnerSpacing.x + 5, 0),
			pos + ImVec2(boxSize + style.ItemInnerSpacing.x + labelSize.x, boxSize));
		ImRect totalBB(checkboxBB.Min, textBB.Max);
		ImGui::ItemSize(totalBB, style.FramePadding.y);
		if (!ImGui::ItemAdd(totalBB, id))
			return false;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(totalBB, id, &hovered, &held);
		static std::map<ImGuiID, CheckState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, CheckState{} });
			itAnim = anim.find(id);
		}
		CheckState& st = itAnim->second;
		float targetFill = *v ? 1.0f : 0.0f;
		st.fillProgress = ImLerp(st.fillProgress, targetFill, g.IO.DeltaTime * 6.f);
		float radialTarget = *v ? 1.0f : 0.0f;
		st.iconAlpha = ImLerp(st.iconAlpha, radialTarget, g.IO.DeltaTime * 6.f);

		if (pressed) {
			*v = !(*v);
			ImGui::MarkItemEdited(id);
		}
		ImU32 borderColor = hovered ? Menu.TextColor : Menu.PageTextColor;
		window->DrawList->AddCircle(checkboxBB.GetCenter(), boxSize * 0.5f, borderColor, 32, 2.0f);
		if (st.fillProgress > 0.01f) {
			float radius = (boxSize * 0.5f - padding) * st.fillProgress;
			ImU32 fillCol = Menu.CheckboxMarkColor;
			window->DrawList->AddCircleFilled(checkboxBB.GetCenter(), radius, fillCol, 32);
		}
		ImU32 textColor = *v ? Menu.TextActiveColor : Menu.TextColor;
		window->DrawList->AddText(textBB.Min, textColor, label);
		return pressed;
	}

	void RadialButtonSelector(const char* label, int* selectedIndex, const char* items[], int itemCount) {
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::BeginGroup();
		for (int i = 0; i < itemCount; i++) {
			bool selected = (*selectedIndex == i);
			ImGui::PushID(i);
			ImRect hoverBB = ImGui::GetCurrentContext()->LastItemData.Rect;
			hoverBB.Max.x += 20;
			bool isHovered = ImGui::IsMouseHoveringRect(hoverBB.Min, hoverBB.Max);
			ImVec4 textColor = Menu.ChildCapColor;
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			if (RadialCheckbox(items[i], &selected)) {
				*selectedIndex = i;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine(0, 15);
			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::PopID();
	}

	inline bool RoundAwesomeCheckbox(const char* label, bool* v, ImVec2 center, float radius = 15.f) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (!window || window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		ImGuiID id = window->GetID(label);
		ImRect circleBB(center - ImVec2(radius, radius), center + ImVec2(radius, radius));
		ImGui::ItemSize(circleBB);
		if (!ImGui::ItemAdd(circleBB, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(circleBB, id, &hovered, &held);
		struct RoundCheckAnim
		{
			float progress = 0.f;
			float checkScale = 0.f;
			float outline = 0.f;
		};
		static std::unordered_map<ImGuiID, RoundCheckAnim> s_AnimMap;
		auto it = s_AnimMap.find(id);
		if (it == s_AnimMap.end())
		{
			s_AnimMap.insert({ id, RoundCheckAnim{} });
			it = s_AnimMap.find(id);
		}
		RoundCheckAnim& anim = it->second;
		float target = (*v ? 1.f : 0.f);
		float dt = g.IO.DeltaTime;
		anim.progress = ImLerp(anim.progress, target, dt * 10.f);
		anim.checkScale = ImLerp(anim.checkScale, target, dt * 16.f);
		anim.outline = ImLerp(anim.outline, (hovered || *v) ? 1.f : 0.f, dt * 10.f);
		ImVec4 offColor = ImVec4(0.85f, 0.85f, 0.85f, 1.f);
		ImVec4 hoverColor = ImVec4(0.70f, 0.70f, 0.70f, 1.f);
		ImVec4 onColor = ImVec4(0.95f, 0.20f, 0.20f, 1.f);
		ImVec4 finalColor;
		if (*v)
		{
			finalColor = ImLerp(offColor, onColor, anim.progress);
		}
		else
		{
			ImVec4 baseOff = hovered ? hoverColor : offColor;
			finalColor = baseOff;
		}
		ImU32 circleColor = ImGui::GetColorU32(finalColor);
		window->DrawList->AddCircleFilled(center, radius, circleColor, 64);
		if (anim.outline > 0.01f)
		{
			float outlineThickness = anim.outline * 2.f;
			ImU32 outlineColor = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, anim.outline * 0.5f));
			window->DrawList->AddCircle(center, radius + 1.f + anim.outline, outlineColor, 64, outlineThickness);
		}
		ImGui::PushFont(Menu.FontAwesome);
		const char* iconOn = ICON_FA_CHECK;
		const char* iconOff = ICON_FA_XMARK;
		const char* icon = *v ? iconOn : iconOff;
		float alpha = (*v ? anim.progress : (1.f - anim.progress)) * 255.f;
		float scale = 0.5f + 0.5f * anim.checkScale;
		ImU32 textColor = IM_COL32(255, 255, 255, (int)alpha);
		ImVec2 textSize = ImGui::CalcTextSize(icon);
		ImVec2 scaledSize(textSize.x * scale, textSize.y * scale);
		ImVec2 textPos;
		if (icon == ICON_FA_CHECK) {
			textPos = ImVec2((center.x - scaledSize.x * 0.5f) + 1, (center.y - scaledSize.y * 0.5f) + 3);
		}
		else {
			textPos = ImVec2((center.x - scaledSize.x * 0.5f) - 2, (center.y - scaledSize.y * 0.5f) - 1);
		}	
		float oldFontSize = Menu.FontAwesome->FontSize;
		Menu.FontAwesome->FontSize = oldFontSize * scale;
		window->DrawList->AddText(Menu.FontAwesome, Menu.FontAwesome->FontSize, textPos, textColor, icon);
		Menu.FontAwesome->FontSize = oldFontSize;
		ImGui::PopFont();
		if (pressed) {
			*v = !(*v);
			ImGui::MarkItemEdited(id);
		}
		return pressed;
	}

	static float CalcMaxPopupHeightFromItemCount(int itemsCount) {
		ImGuiContext& g = *GImGui;
		if (itemsCount <= 0) {
			return FLT_MAX;
		}
		return (g.FontSize + g.Style.ItemSpacing.y) * itemsCount - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	int rotationStartIndex;

	void ImRotateStart() {
		rotationStartIndex = ImGui::GetWindowDrawList()->VtxBuffer.Size;
	}

	ImVec2 ImRotationCenter() {
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);
		const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotationStartIndex; i < buf.Size; i++) {
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);
		}
		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}

	void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter()) {
		float s = sin(rad), c = cos(rad);
		center = ImRotate(center, s, c) - center;
		auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotationStartIndex; i < buf.Size; i++) {
			buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
		}
	}

	bool BeginCombo(const char* label, const char* previewValue, int val, bool multi, ImGuiComboFlags flags, bool hovered = false, bool held = false, bool pressed = false) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		g.NextWindowData.ClearFlags();
		if (window->SkipItems) {
			return false;
		}

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const float w = ((ImGui::GetContentRegionMax().x - style.WindowPadding.x));
		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, 32));
		const ImRect totalBb(bb.Min, bb.Max);

		ImGui::ItemSize(bb, 0.f);
		if (!ImGui::ItemAdd(totalBb, id, &bb))
			return false;

		static std::map<ImGuiID, BeginState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, BeginState() });
			itAnim = anim.find(id);
		}

		if (hovered && g.IO.MouseClicked[0] || itAnim->second.openedCombo && g.IO.MouseClicked[0] && !itAnim->second.hovered)
			itAnim->second.openedCombo = !itAnim->second.openedCombo;

		itAnim->second.arrowRoll = ImLerp(itAnim->second.arrowRoll, itAnim->second.openedCombo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
		itAnim->second.text = ImLerp(itAnim->second.text, itAnim->second.openedCombo ? Menu.TextActiveColor : hovered ? Menu.TextHoverColor : Menu.TextColor, g.IO.DeltaTime * 6.f);
		itAnim->second.background = ImLerp(itAnim->second.background, itAnim->second.openedCombo ? Menu.ElementsHoverColor : Menu.ElementsColor, g.IO.DeltaTime * 6.f);
		itAnim->second.comboSize = ImLerp(itAnim->second.comboSize, itAnim->second.openedCombo ? (val * 30) + 15 : 0.f, g.IO.DeltaTime * 12.f);

		ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(itAnim->second.background), Menu.ElementsRounding);
		ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + style.ItemInnerSpacing.x, bb.Min.y + (32 - labelSize.y) / 2), ImGui::GetColorU32(itAnim->second.text), label);
		ImGui::GetWindowDrawList()->AddRectFilled(bb.Max - ImVec2(32, 32), bb.Max, ImGui::GetColorU32(itAnim->second.background), Menu.ElementsRounding);
		AddRectFilledMultiColor(bb.Max - ImVec2(120, 32), bb.Max, GetColorU32(itAnim->second.background, 0.f), GetColorU32(itAnim->second.background, 1.f), GetColorU32(itAnim->second.background, 1.f), GetColorU32(itAnim->second.background, 0.f), Menu.ElementsRounding);
		ImGui::PushFont(Menu.FontAwesome);
		ImRotateStart();
		ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Max.x - (32 + ImGui::CalcTextSize(ICON_FA_CHEVRON_DOWN).y) / 2, bb.Min.y + (32 - ImGui::CalcTextSize(ICON_FA_CHEVRON_DOWN).y) / 2), Menu.TextColor, ICON_FA_CHEVRON_DOWN);
		ImRotateEnd(1.57f * itAnim->second.arrowRoll);
		ImGui::PopFont();

		if (!ImGui::IsRectVisible(bb.Min, bb.Max + ImVec2(0, 2))) {
			itAnim->second.openedCombo = false;
			itAnim->second.comboSize = 0.f;
		}

		if (!itAnim->second.openedCombo && itAnim->second.comboSize < 2.f) {
			return false;
		}

		ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 5));
		ImGui::SetNextWindowSize(ImVec2(bb.GetWidth(), itAnim->second.comboSize + 15));
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorToImVec4(Menu.ElementsColor));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColorToImVec4(Menu.ElementsColor));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Menu.ElementsRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		bool ret = ImGui::Begin(label, NULL, windowFlags);
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(1);
		ImGui::PopStyleColor();
		itAnim->second.hovered = ImGui::IsWindowHovered();
		if (multi && itAnim->second.hovered && g.IO.MouseClicked[0]) {
			itAnim->second.openedCombo = false;
		}
		return ret;
	}

	bool BeginSettingsPopup(const char* iconLabel, const char* popupId, const ImVec2& size = ImVec2(0, 0)) {
		struct PopupAnim_t
		{
			bool   PopupActive = false;
			float  GearRotation = 0.f;
			ImVec4 IconColor = ImColor(60, 60, 60);
			float  PopupAlpha = 0.f;
		};

		static std::unordered_map<ImGuiID, PopupAnim_t> s_AnimMap;
		static std::unordered_map<ImGuiID, double> popupStartTimes;

		ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
		if (!pWindow || pWindow->SkipItems)
			return false;

		ImGui::PushFont(Menu.FontAwesome);
		ImVec2 iconSize = ImGui::CalcTextSize(iconLabel);
		float baseSize = 14.0f;
		float scaleFactor = baseSize / iconSize.x;
		ImVec2 buttonSize = ImVec2(baseSize, baseSize);

		ImGui::PushID(popupId);
		ImGui::InvisibleButton("IconButton", buttonSize);
		ImVec2 iconPos = ImGui::GetItemRectMin();
		bool hovered = ImGui::IsItemHovered();
		bool clicked = ImGui::IsItemClicked();
		ImGui::PopID();
		ImGui::PopFont();

		ImGuiID popupIdHash = ImGui::GetID(popupId);
		auto itAnim = s_AnimMap.find(popupIdHash);
		if (itAnim == s_AnimMap.end())
		{
			s_AnimMap.insert({ popupIdHash, PopupAnim_t{} });
			itAnim = s_AnimMap.find(popupIdHash);
		}
		PopupAnim_t& animData = itAnim->second;
		if (clicked)
		{
			ImGui::OpenPopup(popupId);
			popupStartTimes[popupIdHash] = ImGui::GetTime();
		}

		bool isPopupOpen = ImGui::IsPopupOpen(popupId);
		animData.PopupActive = isPopupOpen;
		ImGuiContext& g = *GImGui;
		float delta = g.IO.DeltaTime;
		ImVec4 defaultColor = ImColor(Menu.TextColor);
		ImVec4 hoverColor = ImColor(Menu.TextHoverColor);
		ImVec4 activeColor = ImColor(Menu.CheckboxMarkColor);
		ImVec4 targetColor = defaultColor;

		if (animData.PopupActive)
			targetColor = activeColor;
		else if (hovered)
			targetColor = hoverColor;

		animData.IconColor = ImLerp(animData.IconColor, targetColor, delta * 10.f);
		animData.GearRotation = ImLerp(animData.GearRotation, animData.PopupActive ? 1.f : -1.f, delta * 10.f);
		ImRotateStart();
		pWindow->DrawList->AddText(Menu.FontAwesome, Menu.FontAwesome->FontSize * scaleFactor, iconPos, ImGui::GetColorU32(animData.IconColor), iconLabel);
		ImRotateEnd(1.57f * animData.GearRotation);
		animData.PopupAlpha = ImClamp(animData.PopupAlpha + (6.f * delta * (animData.PopupActive ? 1.f : -1.f)), 0.f, 1.f);
		if (isPopupOpen)
		{
			double elapsedTime = ImGui::GetTime() - popupStartTimes[popupIdHash];
			float animationDuration = 0.3f;
			float animProgress = static_cast<float>(elapsedTime / animationDuration);
			if (animProgress > 1.f) animProgress = 1.f;

			ImVec2 startPos = iconPos + ImVec2(0, 30);
			ImVec2 endPos = iconPos + ImVec2(0, 20);
			ImVec2 windowPos = ImLerp(startPos, endPos, animProgress);

			ImGui::SetNextWindowPos(windowPos);
			ImGui::SetNextWindowBgAlpha(animProgress * ImGui::GetStyle().Colors[ImGuiCol_PopupBg].w);
			if (size.x > 0)
			{
				if (size.y > 0) ImGui::SetNextWindowSize(size);
				else ImGui::SetNextWindowSize(ImVec2(size.x, 0.f));
			}
		}
		else
		{
			popupStartTimes.erase(popupIdHash);
		}
		bool opened = false;
		if (animData.PopupAlpha > 0.001f)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, animData.PopupAlpha);
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorToImVec4(Menu.BackgroundColor));
			opened = ImGui::BeginPopup(popupId, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColorToImVec4(Menu.HeaderColor));
			opened = ImGui::BeginPopup(popupId, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::PopStyleColor();
		}

		if (opened) return true;
		return false;
	}

	void EndSettingsPopup() {
		ImGui::EndPopup();
	}

	void EndCombo() {
		ImGui::End();
	}

	bool SelectTable(const char* label, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0, 0)) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 pos = window->DC.CursorPos;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ImGui::ItemSize(size, 0.0f);

		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth)) size.x = ImMax(label_size.x, max_x - min_x);

		const ImVec2 text_min = pos;
		const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0) {
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
			const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}

		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns) {
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
		const bool item_add = ImGui::ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
		if (span_all_columns) {
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add) {
			return false;
		}

		const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
		if (disabled_item && !disabled_global) {
			ImGui::BeginDisabled();
		}
		if (span_all_columns && window->DC.CurrentColumns) {
			ImGui::PushColumnsBackground();
		}
		else if (span_all_columns && g.CurrentTable) {
			ImGui::TablePushBackgroundChannel();
		}

		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) {
			button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
		}

		if (flags & ImGuiSelectableFlags_NoSetKeyOwner) {
			button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
		}

		if (flags & ImGuiSelectableFlags_SelectOnClick) {
			button_flags |= ImGuiButtonFlags_PressedOnClick;
		}

		if (flags & ImGuiSelectableFlags_SelectOnRelease) {
			button_flags |= ImGuiButtonFlags_PressedOnRelease;
		}

		if (flags & ImGuiSelectableFlags_AllowDoubleClick) {
			button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
		}

		if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap)) {
			button_flags |= ImGuiButtonFlags_AllowOverlap;
		}

		const bool was_selected = selected;
		bool hovered, held, pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);

		if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId) {
			if (g.NavJustMovedToId == id) {
				selected = pressed = true;
			}
		}

		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover))) {
			if (!g.NavCursorVisible && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent) {
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, ImGui::WindowRectAbsToRel(window, bb));
				g.NavHighlightItemUnderNav = true;
			}
		}

		if (pressed) {
			ImGui::MarkItemEdited(id);
		}

		if (selected != was_selected) {
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;
		}

		if (g.NavId == id) {
			ImGui::RenderNavHighlight(bb, id, 2 | ImGuiNavHighlightFlags_NoRounding);
		}

		if (span_all_columns && window->DC.CurrentColumns) {
			ImGui::PopColumnsBackground();
		}
		else if (span_all_columns && g.CurrentTable) {
			ImGui::TablePopBackgroundChannel();
		}

		static std::map<ImGuiID, SelectState> anim;
		auto it_anim = anim.find(id);
		if (it_anim == anim.end()) {
			anim.insert({ id, SelectState() });
			it_anim = anim.find(id);
		}

		it_anim->second.text = ImLerp(it_anim->second.text, selected ? Menu.TextActiveColor : Menu.PageTextColor, g.IO.DeltaTime * 6.f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(it_anim->second.text));
		ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
		ImGui::PopStyleColor();

		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.ItemFlags & 32)) {
			ImGui::CloseCurrentPopup();
		}

		if (disabled_item && !disabled_global) {
			ImGui::EndDisabled();
		}
		return pressed;
	}

	static const char* Items_ArrayGetter(void* data, int idx) {
		const char* const* items = (const char* const*)data;
		return items[idx];
	}

	static const char* Items_SingleStringGetter(void* data, int idx) {
		const char* items_separated_by_zeros = (const char*)data;
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p) {
			if (idx == items_count) {
				break;
			}
			p += strlen(p) + 1;
			items_count++;
		}
		return *p ? p : NULL;
	}

	bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = ImGui::GetContentRegionMax().x - style.WindowPadding.x;
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const ImRect frameBb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(w, 65));

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(frameBb, id, &hovered, &held);

		ImGui::GetWindowDrawList()->AddText(ImVec2(frameBb.Max.x - w, frameBb.Min.y), hovered ? Menu.TextHoverColor : Menu.TextColor, label);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
		const char* preview_value = NULL;

		if (*current_item >= 0 && *current_item < items_count) {
			preview_value = getter(user_data, *current_item);
		}

		if (popup_max_height_in_items != -1 && !(g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint)) {
			ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
		}

		if (!BeginCombo(getter(user_data, *current_item), preview_value, items_count, ImGuiComboFlags_None, 0, hovered, held, pressed)) {
			return false;
		}

		bool value_changed = false;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
		for (int i = 0; i < items_count; i++) {
			const char* item_text = getter(user_data, i);
			if (item_text == NULL)
				item_text = "*Unknown item*";

			ImGui::PushID(i);
			const bool item_selected = (i == *current_item);
			if (CustomImGui::SelectTable(item_text, item_selected) && *current_item != i) {
				value_changed = true;
				*current_item = i;
			}
			if (item_selected) {
				ImGui::SetItemDefaultFocus();
			}
			ImGui::PopID();
		}
		ImGui::PopStyleVar();
		EndCombo();
		if (value_changed) {
			ImGui::MarkItemEdited(g.LastItemData.ID);
		}
		return value_changed;
	}

	bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items) {
		const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
		return value_changed;
	}

	bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items) {
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p) {
			p += strlen(p) + 1;
			items_count++;
		}
		bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
		return value_changed;
	}

	bool SliderScalar(const char* label, ImGuiDataType dataType, void* pData, const void* pMin, const void* pMax, const char* format, int width, ImGuiSliderFlags flags) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const ImRect frameBb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(width, 65));
		const ImRect sliderBb(window->DC.CursorPos + ImVec2(0, 50), window->DC.CursorPos + ImVec2(width, 60));
		const ImRect totalBb(frameBb.Min, frameBb.Max + ImVec2(labelSize.x > 0.0f ? labelSize.x : 0.0f, 0.0f));
		const bool tempInputAllowed = (flags & ImGuiSliderFlags_NoInput) == 0;

		ImGui::ItemSize(ImRect(totalBb.Min, totalBb.Max), 0.0f);

		if (!ImGui::ItemAdd(totalBb, id, &frameBb, tempInputAllowed ? ImGuiItemFlags_Inputable : 0)) {
			return false;
		}

		if (format == NULL) {
			format = ImGui::DataTypeGetInfo(dataType)->PrintFmt;
		}

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(frameBb, id, &hovered, &held);

		ImRect grabBb;

		static std::map<ImGuiID, SliderState> anim;
		auto itAnim = anim.find(id);
		if (itAnim == anim.end()) {
			anim.insert({ id, SliderState() });
			itAnim = anim.find(id);
		}

		itAnim->second.text = ImLerp(itAnim->second.text, g.ActiveId == id ? Menu.TextActiveColor : hovered ? Menu.TextHoverColor : Menu.TextColor, g.IO.DeltaTime * 6.0f);
		const bool valueChanged = ImGui::SliderBehavior(ImRect(frameBb.Min - ImVec2(6, 0), frameBb.Max + ImVec2(12, 0)), id, dataType, pData, pMin, pMax, format, flags, &grabBb);
		ImGui::GetWindowDrawList()->AddRectFilled(sliderBb.Min, sliderBb.Max, Menu.ElementsColor, 20, ImDrawFlags_RoundCornersAll);

		if (valueChanged) {
			ImGui::MarkItemEdited(id);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		char valueBuf[64];
		const char* valueBufEnd = valueBuf + ImGui::DataTypeFormatString(valueBuf, IM_ARRAYSIZE(valueBuf), dataType, pData, format);
		itAnim->second.slow = ImLerp(itAnim->second.slow, grabBb.Min.x - (frameBb.Min.x), g.IO.DeltaTime * 25.0f);
		ImGui::PushClipRect(sliderBb.Min + ImVec2(1, 0), sliderBb.Max, true);
		ImGui::GetWindowDrawList()->AddRectFilled(sliderBb.Min + ImVec2(0, 0), ImVec2(itAnim->second.slow + (sliderBb.Min.x + 2), grabBb.Max.y - 5), Menu.MainColor, 20.0f, ImDrawFlags_RoundCornersAll);
		ImGui::PopClipRect();
		ImGui::GetWindowDrawList()->AddText(ImVec2(frameBb.Max.x - ImGui::CalcTextSize(valueBuf).x, frameBb.Min.y + (30 - ImGui::CalcTextSize(valueBuf).y) / 2), Menu.TextColor, valueBuf);
		ImGui::GetWindowDrawList()->AddText(ImVec2(frameBb.Max.x - width, frameBb.Min.y + (30 - ImGui::CalcTextSize(valueBuf).y) / 2), ImGui::GetColorU32(itAnim->second.text), label);
		return valueChanged;
	}

	bool SliderFloat(const char* label, float* v, float vMin, float vMax, const char* format, int width = ImGui::GetContentRegionMax().x, ImGuiSliderFlags flags = 0) {
		return SliderScalar(label, ImGuiDataType_Float, v, &vMin, &vMax, format, width, flags);
	}

	bool SliderInt(const char* label, int* v, int vMin, int vMax, const char* format, int width = ImGui::GetContentRegionMax().x, ImGuiSliderFlags flags = 0) {
		return SliderScalar(label, ImGuiDataType_S32, v, &vMin, &vMax, format, width, flags);
	}

	static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V) {
		ImGuiContext& g = *GImGui;
		IM_ASSERT(g.ColorEditCurrentID != 0);
		if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) {
			return;
		}

		if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1)) {
			*H = g.ColorEditSavedHue;
		}

		if (*V == 0.0f) {
			*S = g.ColorEditSavedSat;
		}
	}

	bool ColorPicker(const char* label, float col[4], ImGuiColorEditFlags flags, const float* refCol) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImDrawList* drawList = window->DrawList;
		ImGuiStyle& style = g.Style;
		ImGuiIO& io = g.IO;

		const float width = ImGui::CalcItemWidth();
		g.NextItemData.ClearFlags();

		ImGui::PushID(label);
		BeginGroup();

		if (!(flags & ImGuiColorEditFlags_NoSidePreview)) {
			flags |= ImGuiColorEditFlags_NoSmallPreview;
		}

		if (!(flags & ImGuiColorEditFlags_NoOptions)) {
			ImGui::ColorPickerOptionsPopup(col, flags);
		}

		if (!(flags & ImGuiColorEditFlags_PickerMask_)) {
			flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
		}
		if (!(flags & ImGuiColorEditFlags_InputMask_)) {
			flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
		}
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));
		if (!(flags & ImGuiColorEditFlags_NoOptions)) {
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);
		}

		int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
		bool alphaBar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
		ImVec2 pickerPos = window->DC.CursorPos;
		ImVec2 barPos = window->DC.CursorPos + ImVec2(0, 133);
		float squareSz = ImGui::GetFrameHeight();
		float barsWidth = 209.f;
		float svPickerSize = ImMax(barsWidth * 1, width - (alphaBar ? 2 : 1) * (barsWidth + style.ItemInnerSpacing.x)) + 0;
		float svBarSize = 20;
		float bar0PosX = ImGui::GetWindowPos().x + style.WindowPadding.x;
		float bar1PosX = bar0PosX;
		float barsTrianglesHalfSz = IM_FLOOR(barsWidth * 0.20f);

		float backupInitialCol[4];
		memcpy(backupInitialCol, col, components * sizeof(float));

		float H = col[0], S = col[1], V = col[2];
		float R = col[0], G = col[1], B = col[2];
		if (flags & ImGuiColorEditFlags_InputRGB) {

			ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
			ColorEditRestoreHS(col, &H, &S, &V);
		}
		else if (flags & ImGuiColorEditFlags_InputHSV) {
			ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
		}

		bool valueChanged = false, valueChangedH = false, valueChangedSv = false;
		ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
		ImGui::InvisibleButton("sv", ImVec2(svPickerSize, svPickerSize - 80));
		if (ImGui::IsItemActive()) {
			S = ImSaturate((io.MousePos.x - pickerPos.x) / (svPickerSize - 1));
			V = 1.0f - ImSaturate((io.MousePos.y - pickerPos.y) / (svPickerSize - 80));

			if (g.ColorEditSavedColor == ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) {
				H = g.ColorEditSavedHue;
			}
			valueChanged = valueChangedSv = true;
		}

		ImGui::SetCursorScreenPos(ImVec2(bar0PosX, barPos.y));
		ImGui::InvisibleButton("hue", ImVec2(barsWidth, svBarSize));
		if (ImGui::IsItemActive()) {
			H = 1.f - ImSaturate((io.MousePos.x - barPos.x) / (barsWidth - 1));
			valueChanged = valueChangedH = true;
		}

		if (alphaBar) {
			ImGui::SetCursorScreenPos(ImVec2(bar1PosX, barPos.y + 16));
			ImGui::InvisibleButton("alpha", ImVec2(barsWidth, svBarSize));
			if (ImGui::IsItemActive()) {
				col[3] = ImSaturate((io.MousePos.x - barPos.x) / (barsWidth - 1));
				valueChanged = true;
			}
		}

		ImGui::PopItemFlag();
		if (valueChangedH || valueChangedSv) {
			if (flags & ImGuiColorEditFlags_InputRGB) {
				ImGui::ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
				g.ColorEditSavedHue = H;
				g.ColorEditSavedSat = S;
				g.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
			}
			else if (flags & ImGuiColorEditFlags_InputHSV) {
				col[0] = H;
				col[1] = S;
				col[2] = V;
			}
		}

		bool valueChangedFixHueWrap = false;
		if (valueChangedFixHueWrap && (flags & ImGuiColorEditFlags_InputRGB)) {
			float newH, newS, newV;
			ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], newH, newS, newV);
			if (newH <= 0 && H > 0) {
				if (newV <= 0 && V != newV) {
					ImGui::ColorConvertHSVtoRGB(H, S, newV <= 0 ? V * 0.5f : newV, col[0], col[1], col[2]);
				}
				else if (newS <= 0) {
					ImGui::ColorConvertHSVtoRGB(H, newS <= 0 ? S * 0.5f : newS, newV, col[0], col[1], col[2]);
				}
			}
		}

		if (valueChanged) {
			if (flags & ImGuiColorEditFlags_InputRGB) {
				R = col[0];
				G = col[1];
				B = col[2];
				ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
				ColorEditRestoreHS(col, &H, &S, &V);
			}
			else if (flags & ImGuiColorEditFlags_InputHSV) {
				H = col[0];
				S = col[1];
				V = col[2];
				ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
			}
		}

		ImU32 userCol32StripedOfAlpha = ImGui::ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));
		const int styleAlpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
		const ImU32 colBlack = IM_COL32(0, 0, 0, styleAlpha8);
		const ImU32 colWhite = IM_COL32(255, 255, 255, styleAlpha8);
		const ImU32 colMidGrey = IM_COL32(128, 128, 128, styleAlpha8);
		const ImU32 colHues[7] = { IM_COL32(255,0,0,styleAlpha8), IM_COL32(255,0,255,styleAlpha8), IM_COL32(0,0,255,styleAlpha8),IM_COL32(0,255,255,styleAlpha8), IM_COL32(0,255,0,styleAlpha8), IM_COL32(255,255,0,styleAlpha8), IM_COL32(255,0,0,styleAlpha8) };
		ImVec4 hueColorF(1, 1, 1, style.Alpha); ImGui::ColorConvertHSVtoRGB(H, 1, 1, hueColorF.x, hueColorF.y, hueColorF.z);
		ImU32 hueColor32 = ImGui::ColorConvertFloat4ToU32(hueColorF);
		ImVec2 svCursorPos;

		const int vtxIdx0 = drawList->VtxBuffer.Size;
		drawList->AddRectFilled(pickerPos, pickerPos + ImVec2(svPickerSize, svPickerSize - 2 - 80), colWhite, 4.0f);
		const int vtxIdx1 = drawList->VtxBuffer.Size;
		ImGui::ShadeVertsLinearColorGradientKeepAlpha(drawList, vtxIdx0, vtxIdx1, pickerPos, pickerPos + ImVec2(svPickerSize, 0.0f), colWhite, hueColor32);
		AddRectFilledMultiColor(pickerPos, pickerPos + ImVec2(svPickerSize, svPickerSize - 80), 0, 0, colBlack, colBlack, 4.f);
		svCursorPos.x = ImClamp(IM_ROUND(pickerPos.x + ImSaturate(S) * svPickerSize), pickerPos.x, pickerPos.x + svPickerSize - 2);
		svCursorPos.y = ImClamp(IM_ROUND(pickerPos.y + ImSaturate(1 - V) * (svPickerSize - 80)), pickerPos.y + 2, pickerPos.y + svPickerSize - 80);

		static std::map<ImGuiID, PickerState> anim;
		auto itAnim = anim.find(ImGui::GetID(label));
		if (itAnim == anim.end()) {
			anim.insert({ ImGui::GetID(label), PickerState() });
			itAnim = anim.find(ImGui::GetID(label));
		}

		for (int i = 0; i < 6; ++i) {
			AddRectFilledMultiColor(ImVec2(pickerPos.x + i * (barsWidth / 6) - (i == 5 ? 1 : 0), pickerPos.y + 139), ImVec2(pickerPos.x + (i + 1) * (barsWidth / 6) + (i == 0 ? 1 : 0), pickerPos.y + 132 + svBarSize - 7), colHues[i], colHues[i + 1], colHues[i + 1], colHues[i], 10.f, i == 0 ? ImDrawFlags_RoundCornersLeft : i == 5 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);
		}

		float bar0LineX = IM_ROUND(barPos.x + (1.f - H) * barsWidth);
		bar0LineX = ImClamp(bar0LineX, pickerPos.x + 3.f, pickerPos.x + 204.f);
		itAnim->second.hue_bar = ImLerp(itAnim->second.hue_bar, bar0LineX - barPos.x, g.IO.DeltaTime * 24.f);
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(itAnim->second.hue_bar + barPos.x, barPos.y + 9), 6.5f, ImColor(255, 255, 255, 255), 30.f);
		itAnim->second.circle_move = ImLerp(itAnim->second.circle_move, svCursorPos - barPos, g.IO.DeltaTime * 24.f);
		itAnim->second.circle = ImLerp(itAnim->second.circle, valueChangedSv ? 4.f : 7.f, g.IO.DeltaTime * 24.f);
		ImGui::GetForegroundDrawList()->AddCircle(itAnim->second.circle_move + barPos + ImVec2(0, 1), itAnim->second.circle, ImColor(255, 255, 255, 255), 32);

		if (alphaBar) {
			float alpha = ImSaturate(col[3]);
			ImRect bar1Bb(bar1PosX, barPos.y + 20, bar1PosX + barsWidth, barPos.y + 20 + svBarSize);
			AddRectFilledMultiColor(pickerPos + ImVec2(0, 161), pickerPos + ImVec2(barsWidth, 147 + svBarSize), colBlack, userCol32StripedOfAlpha, userCol32StripedOfAlpha, colBlack, 10.f);
			float bar1LineX = IM_ROUND(barPos.x + alpha * barsWidth);
			bar1LineX = ImClamp(bar1LineX, barPos.x, pickerPos.x + 200.f);
			itAnim->second.alpha_bar = ImLerp(itAnim->second.alpha_bar, bar1LineX - barPos.x + 5.f, g.IO.DeltaTime * 24.f);
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(itAnim->second.alpha_bar + barPos.x, bar1Bb.Min.y + 11.0f), 6.5f, ImColor(255, 255, 255, 255), 100.f);
		}

		EndGroup();
		if (valueChanged && memcmp(backupInitialCol, col, components * sizeof(float)) == 0) {
			valueChanged = false;
		}

		if (valueChanged) {
			ImGui::MarkItemEdited(g.LastItemData.ID);
		}

		ImGui::PopID();
		return valueChanged;
	}

	bool ColorButton(const char* descId, const ImVec4& col, ImGuiColorEditFlags flags) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiID id = window->GetID(descId);
		const float defaultSize = ImGui::GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const float width = ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x;
		const ImRect rect(pos, pos + ImVec2(width, 32));
		const ImRect clickable(rect.Min + ImVec2(width - 47, 7), rect.Max - ImVec2(7, 7));
		ImGui::ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 0)));
		if (!ImGui::ItemAdd(rect, id)) {
			return false;
		}

		bool hovered, held, pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held);
		if (flags & ImGuiColorEditFlags_NoAlpha) {
			flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);
		}

		ImVec4 colRgb = col;
		if (flags & ImGuiColorEditFlags_InputHSV) {
			ImGui::ColorConvertHSVtoRGB(colRgb.x, colRgb.y, colRgb.z, colRgb.x, colRgb.y, colRgb.z);
		}
		ImGui::GetWindowDrawList()->AddRectFilled(clickable.Min, clickable.Max, ImGui::GetColorU32(colRgb), Menu.ElementsRounding);
		ImGui::RenderColorRectWithAlphaCheckerboard(window->DrawList, clickable.Min, clickable.Max, ImGui::GetColorU32(colRgb), ImMin(36, 29) / 2.99f, ImVec2(0.f, 0.f), Menu.ElementsRounding);
		return pressed;
	}

	bool ColorEdit(const char* label, ImColor& color, ImGuiColorEditFlags flags) {
		float col[4] = { color.Value.x, color.Value.y, color.Value.z, color.Value.w };
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) {
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const float squareSz = ImGui::GetFrameHeight();
		const float wFull = ImGui::CalcItemWidth();
		const float wButton = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (squareSz + style.ItemInnerSpacing.x);
		const float wInputs = wFull - wButton;
		const char* labelDisplayEnd = ImGui::FindRenderedTextEnd(label);
		g.NextItemData.ClearFlags();

		BeginGroup();
		ImGui::PushID(label);
		const bool setCurrentColorEditId = (g.ColorEditCurrentID == 0);
		if (setCurrentColorEditId) {
			g.ColorEditCurrentID = window->IDStack.back();
		}

		const ImGuiColorEditFlags flagsUntouched = flags;
		if (flags & ImGuiColorEditFlags_NoInputs) {
			flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;
		}

		if (!(flags & ImGuiColorEditFlags_NoOptions)) {
			ImGui::ColorEditOptionsPopup(col, flags);
		}

		if (!(flags & ImGuiColorEditFlags_DisplayMask_)) {
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
		}
		if (!(flags & ImGuiColorEditFlags_DataTypeMask_)) {
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
		}
		if (!(flags & ImGuiColorEditFlags_PickerMask_)) {
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
		}
		if (!(flags & ImGuiColorEditFlags_InputMask_)) {
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
		}
		flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_));
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));

		const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
		const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
		const int components = alpha ? 4 : 3;

		float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
		if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB)) {
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		}
		else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV)) {
			ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
			ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
		}

		int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };
		bool valueChanged = false;
		bool valueChangedAsFloat = false;
		const ImVec2 pos = window->DC.CursorPos;
		const float inputsOffsetX = (style.ColorButtonPosition == ImGuiDir_Left) ? wButton : 0.0f;
		window->DC.CursorPos.x = pos.x + inputsOffsetX;

		if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0) {
			const float wItemOne = ImMax(1.0f, IM_FLOOR((wInputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
			const float wItemLast = ImMax(1.0f, IM_FLOOR(wInputs - (wItemOne + style.ItemInnerSpacing.x) * (components - 1)));
			const bool hidePrefix = (wItemOne <= ImGui::CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
			static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
			static const char* fmtTableInt[3][4] = {
				{ "%3d", "%3d", "%3d", "%3d" },
				{ "R:%3d", "G:%3d", "B:%3d", "A:%3d" },
				{ "H:%3d", "S:%3d", "V:%3d", "A:%3d" }
			};
			static const char* fmtTableFloat[3][4] = {
				{ "%0.3f", "%0.3f", "%0.3f", "%0.3f" },
				{ "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" },
				{ "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }
			};
			const int fmtIdx = hidePrefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;
			for (int n = 0; n < components; n++) {
				if (n > 0)
					ImGui::SameLine(0, style.ItemInnerSpacing.x);
				ImGui::SetNextItemWidth((n + 1 < components) ? wItemOne : wItemLast);
				if (flags & ImGuiColorEditFlags_Float) {
					valueChanged |= ImGui::DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmtTableFloat[fmtIdx][n]);
					valueChangedAsFloat |= valueChanged;
				}
				else {
					valueChanged |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmtTableInt[fmtIdx][n]);
				}
				if (!(flags & ImGuiColorEditFlags_NoOptions)) {
					ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
				}
			}
		}
		else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0) {
			if (!(flags & ImGuiColorEditFlags_NoOptions)) {
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
			}
		}

		char buf[64];
		ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
		ImGui::SetNextItemWidth(wInputs);

		ImGuiWindow* pickerActiveWindow = NULL;
		if (!(flags & ImGuiColorEditFlags_NoSmallPreview)) {
			const float buttonOffsetX = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : wInputs + style.ItemInnerSpacing.x;
			window->DC.CursorPos = ImVec2(pos.x + buttonOffsetX, pos.y);
			const ImVec4 colV4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
			if (ColorButton(label, colV4, flags)) {
				if (!(flags & ImGuiColorEditFlags_NoPicker)) {
					g.ColorPickerRef = colV4;
					ImGui::OpenPopup("picker");
					ImGui::SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
				}
			}
			if (!(flags & ImGuiColorEditFlags_NoOptions)) {
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
			}

			ImGui::PushStyleColor(ImGuiCol_PopupBg, GetColorU32(Menu.ElementsColor, 255));
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, Menu.ChildRounding);
			if (ImGui::BeginPopup("picker")) {
				if (g.CurrentWindow->BeginCount == 1) {
					pickerActiveWindow = g.CurrentWindow;
					ImGuiColorEditFlags pickerFlagsToForward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
					ImGuiColorEditFlags pickerFlags = (flagsUntouched & pickerFlagsToForward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
					ImGui::SetNextItemWidth(squareSz * 15.0f);
					valueChanged |= ColorPicker("##picker", col, pickerFlags, &g.ColorPickerRef.x);
				}
				ImGui::EndPopup();
			}
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar();
		}

		if (label != labelDisplayEnd && !(flags & ImGuiColorEditFlags_NoLabel)) {
			static std::map<ImGuiID, EditState> anim;
			auto itAnim = anim.find(ImGui::GetID(label));
			if (itAnim == anim.end()) {
				anim.insert({ ImGui::GetID(label), EditState() });
				itAnim = anim.find(ImGui::GetID(label));
			}

			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
			window->DC.CursorPos.x = pos.x - wButton + ((flags & ImGuiColorEditFlags_NoInputs) ? wButton : wFull);
			ImGui::GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(0, (32 - ImGui::CalcTextSize(buf).y) / 2), GetColorU32(Menu.TextColor, 255), label);
		}

		if (valueChanged && pickerActiveWindow == NULL) {
			if (!valueChangedAsFloat) {
				for (int n = 0; n < 4; n++) {
					f[n] = i[n] / 255.0f;
				}
			}

			if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB)) {
				g.ColorEditSavedHue = f[0];
				g.ColorEditSavedSat = f[1];
				ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
				g.ColorEditSavedID = g.ColorEditCurrentID;
				g.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
			}
			if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV)) {
				ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
			}

			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha) {
				col[3] = f[3];
			}
		}

		if (setCurrentColorEditId) {
			g.ColorEditCurrentID = 0;
		}
		ImGui::PopID();
		EndGroup();

		if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropTarget()) {
			bool acceptedDragDrop = false;
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F)) {
				memcpy((float*)col, payload->Data, sizeof(float) * 3);
				valueChanged = acceptedDragDrop = true;
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F)) {
				memcpy((float*)col, payload->Data, sizeof(float) * components);
				valueChanged = acceptedDragDrop = true;
			}

			if (acceptedDragDrop && (flags & ImGuiColorEditFlags_InputHSV)) {
				ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
			}
			ImGui::EndDragDropTarget();
		}

		if (pickerActiveWindow && g.ActiveId != 0 && g.ActiveIdWindow == pickerActiveWindow) {
			g.LastItemData.ID = g.ActiveId;
		}

		if (valueChanged && g.LastItemData.ID != 0) {
			ImGui::MarkItemEdited(g.LastItemData.ID);
		}

		color = ImColor(col[0], col[1], col[2], col[3]);
		return valueChanged;
	}

	void SeparatorLine() {
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 1), GetColorU32(Menu.SeparatorColor, 255));
		ImGui::Spacing();
	}

	bool IconInputText(const char* label, char* buf, size_t buf_size, const char* icon, ImGuiInputTextFlags flags = 0)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (!window || window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const float input_width = ImGui::GetContentRegionAvail().x;
		const float input_height = 40.0f;
		ImVec2 pos = window->DC.CursorPos;
		ImRect bb(pos, ImVec2(pos.x + input_width, pos.y + input_height));

		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		static std::map<ImGuiID, float> linePulseMap;
		float& linePulse = linePulseMap[id];

		bool hovered = ImGui::IsItemHovered();
		bool active = (ImGui::GetActiveID() == id);

		static std::map<ImGuiID, std::string> lastTextMap;
		std::string curText(buf);
		if (lastTextMap[id] != curText)
		{

			linePulse = 1.0f;
			lastTextMap[id] = curText;
		}

		float dt = g.IO.DeltaTime;
		linePulse = ImLerp(linePulse, 0.0f, dt * 5.f);

		ImU32 bgColor = Menu.HeaderColor;
		ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, bgColor, 8.f);

		ImVec4 baseLine = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);

		ImVec4 lineColor = ImLerp(baseLine, ImVec4(1.0f, 0.3f, 0.3f, 1.f), linePulse * 0.2f);

		float thickness = 2.0f + (linePulse * 3.0f);

		if (hovered) {
			lineColor.x += 0.05f;
			lineColor.y += 0.05f;
			lineColor.z += 0.05f;
		}

		if (active) {
			lineColor.x += 0.05f;
			lineColor.y += 0.05f;
		}

		lineColor.x = ImClamp(lineColor.x, 0.f, 1.f);
		lineColor.y = ImClamp(lineColor.y, 0.f, 1.f);
		lineColor.z = ImClamp(lineColor.z, 0.f, 1.f);

		ImU32 finalLineColor = ImGui::ColorConvertFloat4ToU32(lineColor);



		ImGui::PushFont(Menu.FontAwesome);
		ImVec2 iconSize = ImGui::CalcTextSize(icon);
		ImVec2 iconPos(
			bb.Min.x + 10,
			bb.Min.y + (input_height - iconSize.y) * 0.5f
		);
		ImGui::GetWindowDrawList()->AddText(iconPos, IM_COL32(200, 200, 200, 255), icon);
		ImGui::PopFont();

		float textOffsetY = (input_height - g.FontSize) * 0.5f;
		ImGui::SetCursorScreenPos(ImVec2(bb.Min.x + 40.f, bb.Min.y + textOffsetY));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
		bool changed = ImGui::InputText("##input", buf, buf_size, flags);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (buf[0] == '\0')
		{
			ImU32 placeCol = IM_COL32(160, 160, 160, 200);
			ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 40.f, bb.Min.y + textOffsetY), placeCol, label);
		}

		return changed;
	}

	bool SoftModernListBox(const char* label, int* current_item, const std::vector<std::string>& items, const ImVec2& size = ImVec2(0, 0)) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImVec2 listbox_size = size;
		if (listbox_size.x == 0) listbox_size.x = ImGui::GetContentRegionAvail().x;
		if (listbox_size.y == 0) listbox_size.y = 200.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (Child2(label, listbox_size, true)) {
			for (int i = 0; i < items.size(); ++i) {
				ImGui::PushID(i);

				bool is_selected = (i == *current_item);
				if (i > 0) {
					ImGui::Dummy(ImVec2(0, 0.5f));
				}

				if (SelectTable(items[i].c_str(), is_selected, ImGuiSelectableFlags_SelectOnClick, ImVec2(listbox_size.x, 0))) {
					*current_item = i;
				}

				ImGui::PopID();
			}

			EndChild();
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
		return true;
	}
}