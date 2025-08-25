#include "Fonts.hpp"
#include "Logo.hpp"
#include "PrewiewModel.hpp"

struct MenuStruct {
	ImVec2 Pos, Region, Spacing;
	ImVec2 WindowSize = { 900, 600 };
	ImFont* Inter;
	ImFont* InterSmaller;
	ImFont* InterSemiBold;
	ImFont* FontAwesome;
	ImFont* Arial;
	ImFont* Tahoma;
	ImFont* Verdana;
	ImFont* SegoeUI;
	ImFont* TimesNewRoman;
	ImFont* Calibri;
	ImFont* CourierNew;
	ImFont* Consolas;
	ImFont* TrebuchetMS;
	ImFontConfig Config;
	ID3D11ShaderResourceView* Logo;
	ID3D11ShaderResourceView* Chracter;
	DWORD ColorPickerFlags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha;
	int CurrentTab, SubTab1 = 0, SubTab2 = 0, SubTab3, SubTab4 = 0, SubTab5 = 0, SubTab6 = 0, SubTab7 = 0;
	float BackgroundRounding = 10.f, ChildRounding = 11.f, PageRounding = 4.f, ElementsRounding = 2.f;
	float ButtonHeight = 35.0f;
	float ButtonSpacing = 2.0f;
	float ButtonX = 15.0f;
	float SeparatorOffset = 3.0f;
	float InitialOffsetX = 0;
	int LastTab = -1;
	float TabAlpha = 0.0f;
	float MenuAlpha = 0.0f;
	const float AlphaSpeed = 0.02f;
	ImColor MainColor, BackgroundColor, HeaderColor, TabColor, SeparatorColor, ChildColor, ChildCapColor, ChildCapstructColor, PageActiveColor, PageActiveIconColor, PageColor, PageTextColor, ElementsColor, ElementsHoverColor, TextColor, TextActiveColor, TextHoverColor, AccentColor, CheckboxMarkColor;
} Menu;

void InitializeMenu(ID3D11Device* pDevice) {
	D3DX11_IMAGE_LOAD_INFO info;
	ID3DX11ThreadPump* pump{ nullptr };

	D3DX11CreateShaderResourceViewFromMemory(pDevice, Logo, sizeof(Logo), &info, pump, &Menu.Logo, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, Chracter, sizeof(Chracter), &info, pump, &Menu.Chracter, 0);

	auto& io = ImGui::GetIO();
	auto& style = ImGui::GetStyle();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig iconsConfig;
	iconsConfig.MergeMode = true;
	iconsConfig.PixelSnapH = true;
	iconsConfig.OversampleH = 3;
	iconsConfig.OversampleV = 3;

	Menu.Inter = io.Fonts->AddFontFromMemoryTTF(Inter, sizeof(Inter), 18.0f, &Menu.Config, io.Fonts->GetGlyphRangesCyrillic());
	Menu.InterSmaller = io.Fonts->AddFontFromMemoryTTF(Inter, sizeof(Inter), 14.0f, &Menu.Config, io.Fonts->GetGlyphRangesCyrillic());
	Menu.FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesomeData, FontAwesomeDataSize, 18.f, &iconsConfig, iconsRanges);
	Menu.InterSemiBold = io.Fonts->AddFontFromMemoryTTF(EspFont, sizeof(EspFont), 18.0f, &Menu.Config, io.Fonts->GetGlyphRangesCyrillic());
	Menu.Arial = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\arial.ttf"), 14.0f);
	Menu.Tahoma = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\tahoma.ttf"), 15.0f);
	Menu.Verdana = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\verdana.ttf"), 16.0f);
	Menu.SegoeUI = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\segoeui.ttf"), 17.0f);
	Menu.TimesNewRoman = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\times.ttf"), 18.0f);
	Menu.Calibri = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\calibri.ttf"), 19.0f);
	Menu.CourierNew = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\cour.ttf"), 13.0f);
	Menu.Consolas = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\consola.ttf"), 15.0f);
	Menu.TrebuchetMS = io.Fonts->AddFontFromFileTTF(XorString("C:\\Windows\\Fonts\\trebuc.ttf"), 14.0f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	style.WindowPadding = ImVec2(0, 0);
	style.ItemSpacing = ImVec2(20, 20);
	style.WindowBorderSize = 0;
	style.ScrollbarSize = 1.f;

	Menu.MainColor = ImColor(223, 32, 77);
	Menu.BackgroundColor = ImColor(31, 34, 42);
	Menu.HeaderColor = ImColor(42, 45, 58);
	Menu.TabColor = ImColor(20, 24, 29);
	Menu.AccentColor = ImColor(49, 49, 49);
	Menu.ElementsHoverColor = ImColor(24, 30, 35);
	Menu.ElementsColor = ImColor(20, 24, 29);
	Menu.CheckboxMarkColor = ImColor(223, 32, 77);
	Menu.ChildColor = ImColor(20, 24, 29);
	Menu.ChildCapColor = ImColor(42, 45, 58);
	Menu.ChildCapstructColor = ImColor(31, 31, 39);
	Menu.PageTextColor = ImColor(140, 140, 140);
	Menu.PageActiveColor = ImColor(31, 34, 42);
	Menu.PageActiveIconColor = ImColor(171, 170, 186);
	Menu.PageColor = ImColor(22, 23, 25);
	Menu.TextActiveColor = ImColor(255, 255, 255);
	Menu.TextHoverColor = ImColor(255, 255, 255);
	Menu.TextColor = ImColor(174, 174, 174);
	Menu.SeparatorColor = ImColor(36, 39, 49);
}

#include "Items.hpp"
namespace Cheats {
	namespace MenuUtils {
		int MenuKey = VK_INSERT;
		const char* Fonts[] = { "Original", "Arial", "Tahoma", "Verdana", "Segoe UI", "Times New Roman", "Calibri", "Courier New", "Consolas" };
		int SelectedFontIndex = 0;
		bool StreamProof = false;
	}

	namespace AimAssist {
		bool OnlyVisible = false;
		bool IgnorePed = true;
		bool IgnoreDeath = true;
		static int SelectedMode = 0;
		const char* ModesAimAssist[] = { "Aimbot", "Silent", "T.Bot" };

		namespace Aimbot {
			bool Enabled = false;
			bool DrawFov = false;
			ImColor Color = ImColor(255, 255, 255);
			const char* Type[7] = { "Head", "Neck", "Chest", "RightHand", "LeftHand", "RightAnkle", "LeftAnkle" };

			int SelectedType = 0;
			int Key;
			int Fov = 50;
			int Smooth = 5;
			int Distance = 300;
		}

		namespace Silent {
			bool Enabled = false;
			bool RandomTarget = false;
			bool DrawFov = false;
			ImColor Color = ImColor(255, 255, 255);
			const char* Type[7] = { "Head", "Neck", "Chest", "RightHand", "LeftHand", "RightAnkle", "LeftAnkle" };

			int SelectedType = 0;
			bool Pslient = false;
			int Key;
			int Fov = 50;
			int Smooth = 5;
			int Distance = 300;
		}

		namespace Triggerbot {
			bool Enabled = false;
			bool DrawFov = false;
			ImColor Color = ImColor(255, 255, 255);
			const char* Type[7] = { "Head", "Neck", "Chest", "RightHand", "LeftHand", "RightAnkle", "LeftAnkle" };

			int SelectedType = 0;
			int Key;
			int Fov = 50;
			int Smooth = 5;
			int Delay = 1;
			int CrosshairTolerance = 5;
			int Distance = 300;
		}
	}

	namespace Players {
		bool OnlyVisible = false;
		bool IgnorePed = true;
		bool IgnoreDeath = true;
		int Distance = 300;

		namespace DrawSkeleton {
			bool Enabled = false;
			ImColor Color = ImColor(255, 255, 255);
		}

		namespace DrawId {
			bool Enabled = false;
			ImColor Color = ImColor(255, 255, 255);
		}

		namespace DrawName {
			bool Enabled = false;
			ImColor Color = ImColor(255, 255, 255);
		}

		namespace DrawBox {
			bool Enabled = false;
			const char* Type[2]{ "2D", "Corner" };
			int SelectedType = 0;
			ImColor Color = ImColor(255, 255, 255);
			float Size = 0.75f;
			bool UseCustomGradient = false;
			ImColor BoxGradientTopColor = ImColor(0, 0, 255, 255);
			ImColor BoxGradientBottomColor = ImColor(255, 0, 0, 255);
			bool GradientEnabled = false;
			float GradientIntensity = 0.0f;
		}

		namespace DrawLine {
			bool Enabled = false;
			const char* Type[3]{ "Top", "Center", "Bottom" };
			int SelectedType = 0;
			ImColor Color = ImColor(255, 255, 255);
		}

		namespace DrawDistance {
			bool Enabled = false;
			bool StyleBg = false;
			ImColor Color = ImColor(255, 255, 255);
		}

		namespace DrawHealth {
			bool Enabled = false;
		}

		namespace DrawArmor {
			bool Enabled = false;
		}

		namespace DrawWeaponName {
			bool Enabled = false;
			ImColor Color = ImColor(255, 255, 255);
		}
	}

	namespace Vehicle {
		ImColor HealthBarColor = ImColor(0, 255, 0, 255);
		ImColor DistanceColor = ImColor(255, 255, 255, 255);
		ImColor SnaplineColor = ImColor(255, 255, 255, 255);
		ImColor MarkerOuterColor = ImColor(0, 0, 0, 100);
		ImColor MarkerInnerColor = ImColor(255, 0, 0, 255);

		bool Enabled = false;
		bool DrawLocalVehicle = false;
		bool DrawEnemyVehicle = false;
		bool VehicleHealth = false;
		bool VehicleEspShowDistance = false;
		bool VehicleEspSnapline = false;
		bool VehicleMarker = false;
		int Distance = 300;
		bool Fix;
		int FixKey;
	}

	namespace Crosshairs {
		bool Enabled = false;
		int SelectedType = 0;
		const char* Type[10]{ "Type 1", "Type 2", "Type 3", "Type 4", "Type 5", "Type 6", "Type 7", "Type 8", "Type 9", "Type 10" };
		int Size = 10;
		ImColor Color = ImColor(255, 255, 255);
	}

	namespace Exploit {
		bool HealthBoost = false;
		float HealthBoostValue = 200.0f;
		int HealthBoostKey;

		bool ArmorBoost = false;
		float ArmorBoostValue = 20.0f;
		int ArmorBoostKey;

		bool InfiniteAmmo = false;
		bool NoRecoil = false;
		bool NoSpread = false;
		bool NoReload = false;
		bool NoRange = false;
		bool ReloadAmmo = false;
		int ReloadValue = 1;
		int ReloadAmmoKey;
	}

	namespace NoClip {
		bool Enabled = false;
		int Speed;
		int Key;
	}

	namespace Teleport {
		bool TeleportWaypoint = false;
		bool CustomTeleportWaypoint = false;
		float posX = 0.0f, posY = 0.0f;
	}
}

static int SelectedItemVehicle = -1;
static char SearchBufferVehicle[128] = "";
std::vector<std::string> VehicleNames = {};

static std::vector<int> playerIDs;
static std::map<int, bool> friendStatus;
static std::vector<int> newPlayerIDs;
static std::vector<int> oldPlayerIDs = playerIDs;
static std::vector<std::string> playerNames;   

static int selectedPlayerID = -1;
static int selectedItemPlayer = -1;
static char searchBuffer[128] = "";
static bool Teleport = false;

void SelectTarget(int& selectedType) {
	if (!Menu.Chracter) {
		return;
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 childPos = ImGui::GetCursorScreenPos();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImVec2 imageSize = ImVec2(150, 366);

	float imagePosX = childPos.x + (windowSize.x - imageSize.x) * 0.5f;
	float imagePosY = childPos.y + 10.0f;
	ImVec2 imagePos = ImVec2(imagePosX, imagePosY);

	drawList->AddImage((ImTextureID)Menu.Chracter, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

	float cx = imagePos.x + imageSize.x * 0.5f;
	float headY = imagePos.y + 25.0f;
	float Hip = headY + 35.0f;
	float Neck = Hip + 45.0f;
	float armY = Neck + 20.0f;
	float legY = imagePos.y + imageSize.y * 0.72f;
	float leftArmOffset = 50.0f;
	float rightArmOffset = 55.0f;
	float leftLegOffset = 30.0f;
	float rightLegOffset = 40.0f;
	float checkboxRadius = 12.0f;
	bool selectedParts[7] = { false };
	selectedParts[selectedType] = true;

	if (CustomImGui::RoundAwesomeCheckbox(XorString("Head"), &selectedParts[0], ImVec2(cx, headY), checkboxRadius)) {
		selectedType = 0;
	}
		
	if (CustomImGui::RoundAwesomeCheckbox(XorString("Neck"), &selectedParts[1], ImVec2(cx, Hip), checkboxRadius)) {
		selectedType = 1;
	}
		
	if (CustomImGui::RoundAwesomeCheckbox(XorString("Chest"), &selectedParts[2], ImVec2(cx, Neck + 10.0f), checkboxRadius)) {
		selectedType = 2;
	}
		
	if (CustomImGui::RoundAwesomeCheckbox(XorString("Left Arm"), &selectedParts[3], ImVec2(cx - leftArmOffset, armY), checkboxRadius)) {
		selectedType = 3;
	}
		
	if (CustomImGui::RoundAwesomeCheckbox(XorString("Right Arm"), &selectedParts[4], ImVec2(cx + rightArmOffset, armY), checkboxRadius)) {
		selectedType = 4;
	}
		
	if (CustomImGui::RoundAwesomeCheckbox(XorString("Left Leg"), &selectedParts[5], ImVec2(cx - leftLegOffset, legY), checkboxRadius)) {
		selectedType = 5;
	}

	if (CustomImGui::RoundAwesomeCheckbox(XorString("Right Leg"), &selectedParts[6], ImVec2(cx + rightLegOffset, legY), checkboxRadius)) {
		selectedType = 6;
	}		
}

void DrawPreview() {
	if (!Menu.Chracter) return;
	ImGui::PushFont(Menu.InterSemiBold);
 	 
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 childPos = ImGui::GetCursorScreenPos();
	ImVec2 imagePos = ImVec2(childPos.x + 50, childPos.y + 10);
	ImVec2 imageSize = ImVec2(150, 366);

	drawList->AddImage((ImTextureID)Menu.Chracter, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
	if (Cheats::Players::DrawSkeleton::Enabled) {
		float offsetX = 75.0f, offsetY = 60.0f;
		ImVec2 skeletonCoords[] = {
			{1225 + offsetX, 438 + offsetY}, {1185 + offsetX, 450 + offsetY},
			{1186 + offsetX, 450 + offsetY}, {1171 + offsetX, 494 + offsetY},
			{1172 + offsetX, 494 + offsetY}, {1161 + offsetX, 539 + offsetY},
			{1223 + offsetX, 439 + offsetY}, {1268 + offsetX, 451 + offsetY},
			{1268 + offsetX, 452 + offsetY}, {1279 + offsetX, 485 + offsetY},
			{1279 + offsetX, 485 + offsetY}, {1289 + offsetX, 538 + offsetY},
			{1225 + offsetX, 438 + offsetY}, {1229 + offsetX, 553 + offsetY},
			{1229 + offsetX, 553 + offsetY}, {1191 + offsetX, 553 + offsetY},
			{1228 + offsetX, 552 + offsetY}, {1265 + offsetX, 555 + offsetY},
			{1192 + offsetX, 554 + offsetY}, {1198 + offsetX, 629 + offsetY},
			{1198 + offsetX, 629 + offsetY}, {1208 + offsetX, 682 + offsetY},
			{1208 + offsetX, 682 + offsetY}, {1186 + offsetX, 707 + offsetY},
			{1266 + offsetX, 556 + offsetY}, {1262 + offsetX, 634 + offsetY},
			{1262 + offsetX, 634 + offsetY}, {1269 + offsetX, 679 + offsetY},
			{1269 + offsetX, 679 + offsetY}, {1276 + offsetX, 711 + offsetY}
		};

		for (int i = 0; i < (int)(sizeof(skeletonCoords) / sizeof(skeletonCoords[0])); i += 2) {
			ImVec2 start = skeletonCoords[i];
			ImVec2 end = skeletonCoords[i + 1];
			start.x = imagePos.x + ((start.x - 1225) / 150.0f) * imageSize.x;
			start.y = imagePos.y + ((start.y - 438) / 350.0f) * imageSize.y;
			end.x = imagePos.x + ((end.x - 1225) / 150.0f) * imageSize.x;
			end.y = imagePos.y + ((end.y - 438) / 350.0f) * imageSize.y;
			drawList->AddLine(start, end, Cheats::Players::DrawSkeleton::Color, 2.0f);
		}
	}

	if (Cheats::Players::DrawBox::Enabled) {
		if (Cheats::Players::DrawBox::SelectedType == 0) {
			ImVec2 boxTL = imagePos;
			ImVec2 boxBR = ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y);
			drawList->AddRect(boxTL, boxBR, Cheats::Players::DrawBox::Color, 0, 0, 2.0f);
		}
		else if (Cheats::Players::DrawBox::SelectedType == 1) {
			float cornerSize = 15.0f;
			ImVec2 boxTL = imagePos;
			ImVec2 boxBR = ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y);
			ImVec2 boxTR = ImVec2(boxBR.x, boxTL.y);
			ImVec2 boxBL = ImVec2(boxTL.x, boxBR.y);
			drawList->AddLine(boxTL, ImVec2(boxTL.x + cornerSize, boxTL.y), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxTL, ImVec2(boxTL.x, boxTL.y + cornerSize), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxTR, ImVec2(boxTR.x - cornerSize, boxTR.y), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxTR, ImVec2(boxTR.x, boxTR.y + cornerSize), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxBL, ImVec2(boxBL.x + cornerSize, boxBL.y), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxBL, ImVec2(boxBL.x, boxBL.y - cornerSize), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxBR, ImVec2(boxBR.x - cornerSize, boxBR.y), Cheats::Players::DrawBox::Color, 2.0f);
			drawList->AddLine(boxBR, ImVec2(boxBR.x, boxBR.y - cornerSize), Cheats::Players::DrawBox::Color, 2.0f);
		}
	}
 
	if (Cheats::Players::DrawArmor::Enabled) {
		float barHeight = imageSize.y;
		float barWidth = 5.0f;
		ImVec2 barTL = ImVec2(imagePos.x - barWidth - 5, imagePos.y);
		ImVec2 barBR = ImVec2(barTL.x + barWidth, barTL.y + barHeight);
		ImU32 colTop = IM_COL32(30, 144, 255, 255);
		ImU32 colBottom = IM_COL32(135, 206, 250, 255);
		drawList->AddRectFilledMultiColor(barTL, barBR, colTop, colTop, colBottom, colBottom);
		drawList->AddRect(barTL, barBR, IM_COL32(0, 0, 0, 255), 2.0f);
	}

	if (Cheats::Players::DrawName::Enabled) {
		const char* name = XorString("PETROV X VIOS");
		ImVec2 size = ImGui::CalcTextSize(name);
		ImVec2 pos = ImVec2(imagePos.x + (imageSize.x - size.x) / 2 + 2, imagePos.y - 22);
		drawList->AddText(pos, Cheats::Players::DrawName::Color, name);
	}

	float baseY = imagePos.y + imageSize.y;   
	float offset = 5.0f;                      

	if (Cheats::Players::DrawHealth::Enabled) {
 		float barW = imageSize.x;
		float barH = 9.0f;

		ImVec2 barPos(imagePos.x, baseY + offset);         
		ImVec2 barEnd(barPos.x + barW, barPos.y + barH);   

		ImU32 colLeft = IM_COL32(34, 139, 34, 255);
		ImU32 colRight = IM_COL32(50, 205, 50, 255);

		drawList->AddRectFilledMultiColor(barPos, barEnd, colLeft, colRight, colRight, colLeft);
		drawList->AddRect(barPos, barEnd, IM_COL32(0, 0, 0, 255), 2.0f);
		offset += barH + 3.0f;
	}

	if (Cheats::Players::DrawWeaponName::Enabled) {
		const char* weapon = XorString("AK-47");
		ImVec2 wpnSize = ImGui::CalcTextSize(weapon);
		ImVec2 textPos(imagePos.x + (imageSize.x - wpnSize.x) * 0.5f, baseY + offset);		 
		drawList->AddText(textPos, Cheats::Players::DrawWeaponName::Color, weapon);
		offset += wpnSize.y + 3.0f;
	}

	if (Cheats::Players::DrawDistance::Enabled)
	{
		const char* dist = XorString("500m");
		ImVec2 distSize = ImGui::CalcTextSize(dist);
		ImVec2 textPos(imagePos.x + (imageSize.x - distSize.x) * 0.5f, baseY + offset);
		drawList->AddText(textPos, Cheats::Players::DrawDistance::Color, dist);
		offset += distSize.y + 3.0f;
	}
	ImGui::PopFont();	 
}

void TabAimAssist() {
	{
		ImGui::SetCursorPosY(10);
		if (CustomImGui::SubTab(0 == Menu.SubTab1, XorString("Aimbot"), ImVec2(97, 30))) {
			Menu.SubTab1 = 0;
			Cheats::AimAssist::SelectedMode = 0;
		}
		ImGui::SameLine();
		if (CustomImGui::SubTab(1 == Menu.SubTab1, XorString("Silent"), ImVec2(97, 30))) {
			Menu.SubTab1 = 1;
			Cheats::AimAssist::SelectedMode = 1;
		}
		ImGui::SameLine();
		if (CustomImGui::SubTab(2 == Menu.SubTab1, XorString("Triggerbot"), ImVec2(97, 30))) {
			Menu.SubTab1 = 2;
			Cheats::AimAssist::SelectedMode = 2;
		}
	}

	ImGui::SetCursorPos(ImVec2(155, 60));

	if (Menu.SubTab1 == 0) {
		CustomImGui::Child2(XorString("Aimbot SubTab"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Aimbot"), &Cheats::AimAssist::Aimbot::Enabled);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 280);
			CustomImGui::Checkbox(XorString("Only Visible"), &Cheats::AimAssist::OnlyVisible);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Ped"), &Cheats::AimAssist::IgnorePed);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Death"), &Cheats::AimAssist::IgnoreDeath);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Aimbot Fov"), &Cheats::AimAssist::Aimbot::DrawFov);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Settings"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Aimbot Fov Color"), Cheats::AimAssist::Aimbot::Color, Menu.ColorPickerFlags);

				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Keybind(XorString("Aimbot Key"), &Cheats::AimAssist::Aimbot::Key, 0);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Aimbot Fov"), &Cheats::AimAssist::Aimbot::Fov, 1, 400, XorString("%d"));
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Aimbot Smooth"), &Cheats::AimAssist::Aimbot::Smooth, 1, 50, XorString("%d"));
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Aimbot Distance"), &Cheats::AimAssist::Aimbot::Distance, 1, 1000, XorString("%dm"));
		}
		CustomImGui::EndChild();
	}

	if (Menu.SubTab1 == 1) {
		CustomImGui::Child2(XorString("Silent SubTab"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Silent"), &Cheats::AimAssist::Silent::Enabled);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 291);
			CustomImGui::Checkbox(XorString("Only Visible"), &Cheats::AimAssist::OnlyVisible);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Ped"), &Cheats::AimAssist::IgnorePed);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Death"), &Cheats::AimAssist::IgnoreDeath);
			CustomImGui::SeparatorLine();

			CustomImGui::Checkbox(XorString("Draw Silent Fov"), &Cheats::AimAssist::Silent::DrawFov);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Silent Settings"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Silent Fov Color"), Cheats::AimAssist::Silent::Color, Menu.ColorPickerFlags);

				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Random Target"), &Cheats::AimAssist::Silent::RandomTarget);
			CustomImGui::SeparatorLine();
			CustomImGui::Keybind(XorString("Silent Key"), &Cheats::AimAssist::Silent::Key, 0);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Silent Fov"), &Cheats::AimAssist::Silent::Fov, 1, 400, XorString("%d"));
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Silent Distance"), &Cheats::AimAssist::Silent::Distance, 1, 1000, XorString("%dm"));
			CustomImGui::SeparatorLine();
		}
		CustomImGui::EndChild();
	}

	if (Menu.SubTab1 == 2) {
		CustomImGui::Child2(XorString("Triggerbot SubTab"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Triggerbot"), &Cheats::AimAssist::Triggerbot::Enabled);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 256);
			CustomImGui::Checkbox(XorString("Only Visible"), &Cheats::AimAssist::OnlyVisible);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Ped"), &Cheats::AimAssist::IgnorePed);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Death"), &Cheats::AimAssist::IgnoreDeath);
			CustomImGui::SeparatorLine();

			CustomImGui::Checkbox(XorString("Draw Triggerbot Fov"), &Cheats::AimAssist::Triggerbot::DrawFov);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Triggerbot Settings"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Triggerbot Fov Color"), Cheats::AimAssist::Triggerbot::Color, Menu.ColorPickerFlags);

				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Keybind(XorString("Triggerbot Key"), &Cheats::AimAssist::Triggerbot::Key, 0);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Triggerbot Fov"), &Cheats::AimAssist::Triggerbot::Fov, 1, 400, XorString("%d"), 710);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Triggerbot Distance"), &Cheats::AimAssist::Triggerbot::Distance, 1, 1000, XorString("%dm"), 710);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Triggerbot Delay"), &Cheats::AimAssist::Triggerbot::Delay, 1, 10, "%d", 710);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Crosshair Tolerance"), &Cheats::AimAssist::Triggerbot::CrosshairTolerance, 30, 200, "%d",710);
		}
		CustomImGui::EndChild();
	}
}

void TabPlayers() {
	{
		ImGui::SetCursorPosY(10);
		if (CustomImGui::SubTab(0 == Menu.SubTab2, XorString("Players"), ImVec2(97, 30))) {
			Menu.SubTab2 = 0;
		}
	}
	ImGui::SetCursorPos(ImVec2(155, 60));
	if (Menu.SubTab2 == 0) {
		CustomImGui::Child2(XorString("Visuals SubTab"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Draw Skeleton"), &Cheats::Players::DrawSkeleton::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Skeleton"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Skeleton Color"), Cheats::Players::DrawSkeleton::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Id"), &Cheats::Players::DrawId::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Id"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Id Color"), Cheats::Players::DrawId::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Name"), &Cheats::Players::DrawName::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Name"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Name Color"), Cheats::Players::DrawName::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Box"), &Cheats::Players::DrawBox::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Box"), ImVec2(245, 0))) {
				CustomImGui::RadialButtonSelector("", &Cheats::Players::DrawBox::SelectedType, Cheats::Players::DrawBox::Type, IM_ARRAYSIZE(Cheats::Players::DrawBox::Type));
				CustomImGui::SeparatorLine();
				CustomImGui::ColorEdit(XorString("Box Color"), Cheats::Players::DrawBox::Color, Menu.ColorPickerFlags);
				CustomImGui::SliderFloat(XorString("Box Size"), &Cheats::Players::DrawBox::Size, 0.75f, 1.202f, "Value: %.3f", 210);
				CustomImGui::SeparatorLine();
				CustomImGui::SeparatorLine();
				CustomImGui::Checkbox(XorString("Enable Gradient"), &Cheats::Players::DrawBox::GradientEnabled);
				if (Cheats::Players::DrawBox::GradientEnabled) {
					CustomImGui::Checkbox(XorString("Use Custom Gradient"), &Cheats::Players::DrawBox::UseCustomGradient);
					CustomImGui::SliderFloat(XorString("Gradient Intensity"), &Cheats::Players::DrawBox::GradientIntensity, 0.0f, 5.0f, "Value: %.2f", 210);
					if (Cheats::Players::DrawBox::UseCustomGradient) {
						CustomImGui::ColorEdit(XorString("Top Color"), Cheats::Players::DrawBox::BoxGradientTopColor, Menu.ColorPickerFlags);
						CustomImGui::ColorEdit(XorString("Bottom Color"), Cheats::Players::DrawBox::BoxGradientBottomColor, Menu.ColorPickerFlags);
					}
				}
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Line"), &Cheats::Players::DrawLine::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Line"), ImVec2(280, 0))) {
				CustomImGui::RadialButtonSelector("", &Cheats::Players::DrawLine::SelectedType, Cheats::Players::DrawLine::Type, IM_ARRAYSIZE(Cheats::Players::DrawLine::Type));

				CustomImGui::SeparatorLine();
				CustomImGui::ColorEdit(XorString("Line Color"), Cheats::Players::DrawLine::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}

			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Distance"), &Cheats::Players::DrawDistance::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Distance"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Distance Color"), Cheats::Players::DrawDistance::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}

			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Health"), &Cheats::Players::DrawHealth::Enabled);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Armor"), &Cheats::Players::DrawArmor::Enabled);

			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Draw Weapon Name"), &Cheats::Players::DrawWeaponName::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Draw Weapon Name"), ImVec2(225, 0))) {
				CustomImGui::ColorEdit(XorString("Weapon Name Color"), Cheats::Players::DrawWeaponName::Color, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}

			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Players Distance"), &Cheats::Players::Distance, 1, 1000, XorString("%dm"), 330);
			ImGui::SetCursorPos(ImVec2(365, 450));
			CustomImGui::Checkbox(XorString("Only Visible"), &Cheats::Players::OnlyVisible);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Ped"), &Cheats::Players::IgnorePed);
			ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
			CustomImGui::Checkbox(XorString("Ignore Death"), &Cheats::Players::IgnoreDeath);
		}
		CustomImGui::EndChild();
	}
}

void TabVehicles() {
	ImGui::SetCursorPosY(10);
	if (CustomImGui::SubTab(0 == Menu.SubTab6, XorString("Vehicle"), ImVec2(97, 30))) Menu.SubTab6 = 0;
	ImGui::SetCursorPos(ImVec2(155, 60));
	if (Menu.SubTab6 == 0) {
		CustomImGui::Child2(XorString("Vehicle"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Vehicle Esp"), &Cheats::Vehicle::Enabled);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Distance Settings"), ImVec2(200, 0))) {
				CustomImGui::SliderInt(XorString("Draw Distance"), &Cheats::Vehicle::Distance, 30, 200, "%d", 150);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Local Vehicle"), &Cheats::Vehicle::DrawLocalVehicle);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Enemy Vehicle"), &Cheats::Vehicle::DrawEnemyVehicle);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Vehicle Distance"), &Cheats::Vehicle::VehicleEspShowDistance);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Distance Color"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Distance Color"), Cheats::Vehicle::DistanceColor, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();

			CustomImGui::Checkbox(XorString("Vehicle Marker"), &Cheats::Vehicle::VehicleMarker);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Marker Settings"), ImVec2(230, 0))) {
				CustomImGui::ColorEdit(XorString("Marker Outer Color"), Cheats::Vehicle::MarkerOuterColor, Menu.ColorPickerFlags);
				CustomImGui::ColorEdit(XorString("Marker Inner Color"), Cheats::Vehicle::MarkerInnerColor, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();

			CustomImGui::Checkbox(XorString("Vehicle Snapline"), &Cheats::Vehicle::VehicleEspSnapline);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Snapline Settings"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Snapline Color"), Cheats::Vehicle::SnaplineColor, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();

			CustomImGui::Checkbox(XorString("Vehicle Health"), &Cheats::Vehicle::VehicleHealth);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Health Settings"), ImVec2(200, 0))) {
				CustomImGui::ColorEdit(XorString("Health Bar Color"), Cheats::Vehicle::HealthBarColor, Menu.ColorPickerFlags);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Vehicle Fix"), &Cheats::Vehicle::Fix);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Vehicle Fix Settings"), ImVec2(200, 0))) {
				CustomImGui::Keybind(XorString("Vehicle Fix Key"), &Cheats::Vehicle::FixKey, 0);
				ImGui::EndPopup();
			}

		}
		CustomImGui::EndChild();
	}
}

void MapTeleport();
void TabWorld() {
	{
		ImGui::SetCursorPosY(10);
		if (CustomImGui::SubTab(0 == Menu.SubTab3, XorString("Map Teleport"), ImVec2(97, 30))) {
			Menu.SubTab3 = 0;
		}
	}

	ImGui::SetCursorPos(ImVec2(155, 60));
	if (Menu.SubTab3 == 0) {
		CustomImGui::Child2(XorString("Map Teleport"), ImVec2(730, 470), true);
		{
			MapTeleport();
		}
		CustomImGui::EndChild();
	}

	if (Menu.SubTab3 == 1) {
		CustomImGui::Child2(XorString("No Clip"), ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("No Clip"), &Cheats::NoClip::Enabled);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("No Clip Speed"), &Cheats::NoClip::Speed, 0, 100, "%dm/s");
			CustomImGui::SeparatorLine();
			CustomImGui::Keybind(XorString("No Clip Key"), &Cheats::NoClip::Key, 0);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderFloat(XorString("X Position"), &Cheats::Teleport::posX, 0.0f, 10000.0f, "Value: %.3f");
			CustomImGui::SliderFloat(XorString("Y Position"), &Cheats::Teleport::posY, 0.0f, 10000.0f, "Value: %.3f");
			CustomImGui::SeparatorLine();

			ImGui::SetCursorPosX(18);
			if (CustomImGui::Button(XorString("Custom Teleport Waypoint"), ImVec2(700, 30))) {
				Cheats::Teleport::CustomTeleportWaypoint = true;
			}
			CustomImGui::SeparatorLine();
			ImGui::SetCursorPosX(18);

			if (CustomImGui::Button(XorString("Teleport Waypoint"), ImVec2(700, 30))) {
				Cheats::Teleport::TeleportWaypoint = true;
			}
		}
		CustomImGui::EndChild();
	}
}

void TabMisc() {
	{
		ImGui::SetCursorPosY(10);
		if (CustomImGui::SubTab(0 == Menu.SubTab4, XorString("Player"), ImVec2(97, 30))) {
			Menu.SubTab4 = 0;
		}
		ImGui::SameLine();
		if (CustomImGui::SubTab(1 == Menu.SubTab4, XorString("Weapon"), ImVec2(97, 30))) {
			Menu.SubTab4 = 1;
		}
	}

	ImGui::SetCursorPos(ImVec2(155, 60));
	if (Menu.SubTab4 == 0) {
		CustomImGui::Child2(XorString("Player"), ImVec2(730, 470), true);
		{
			CustomImGui::Checkbox(XorString("Player Healt Boos"), &Cheats::Exploit::HealthBoost);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Health Settings"), ImVec2(200, 0))) {
				CustomImGui::Keybind(XorString("Health Boost Key"), &Cheats::Exploit::HealthBoostKey, 0);
				ImGui::EndPopup();
			}
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Player Armor Boost"), &Cheats::Exploit::ArmorBoost);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Armor Settings"), ImVec2(320, 0))) {
				CustomImGui::SliderFloat(XorString("Armor Boost Value"), &Cheats::Exploit::ArmorBoostValue, 20.0f, 100.0f, "Value: %.3f", 280);
				CustomImGui::SeparatorLine();
				CustomImGui::Keybind(XorString("Armor Boost Key"), &Cheats::Exploit::ArmorBoostKey, 0);
				ImGui::EndPopup();
			}
		}
		CustomImGui::EndChild();
	}

	if (Menu.SubTab4 == 1) {
		CustomImGui::Child2("Weapon", ImVec2(730, 500), true);
		{
			CustomImGui::Checkbox(XorString("Infinite Ammo"), &Cheats::Exploit::InfiniteAmmo);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("No Recoil"), &Cheats::Exploit::NoRecoil);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("No Spread"), &Cheats::Exploit::NoSpread);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("No Reload"), &Cheats::Exploit::NoReload);
			CustomImGui::SeparatorLine();
			CustomImGui::Checkbox(XorString("Reload Ammo"), &Cheats::Exploit::ReloadAmmo);
			ImGui::SameLine(0, 35);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			if (CustomImGui::BeginSettingsPopup(ICON_FA_GEAR, XorString("##Reload Settings"), ImVec2(245, 0))) {
				CustomImGui::SliderInt(XorString("Ammo Value"), &Cheats::Exploit::ReloadValue, 1, 30, XorString("%d"), 210);
				CustomImGui::Keybind(XorString("Reload Ammo Key"), &Cheats::Exploit::ReloadAmmoKey, 0);
				ImGui::EndPopup();
			}
		}
		CustomImGui::EndChild();
	}
}

void TabPlayerList() {
	{
		ImGui::SetCursorPosY(10);
 		if (CustomImGui::SubTab(0 == Menu.SubTab5, XorString("Player List"), ImVec2(97, 30))) {
			Menu.SubTab5 = 0;
		}
	}

	ImGui::SetCursorPos(ImVec2(155, 60));
 	if (Menu.SubTab5 == 0) {
		CustomImGui::Child2(XorString("Player List"), ImVec2(730, 500), true);

		if (CustomImGui::IconInputText(XorString("Search"), searchBuffer, sizeof(searchBuffer), ICON_FA_MAGNIFYING_GLASS)) {
			selectedItemPlayer = -1;
			selectedPlayerID = -1;
		}

		CustomImGui::SeparatorLine();
		ImGui::Columns(2, nullptr, false);
		CustomImGui::Child2(XorString("LeftPanel"), ImVec2(0, 0), true);

		std::vector<int> filtered;
		for (int i = 0; i < (int)playerIDs.size(); i++) {
			std::string searchText = std::to_string(playerIDs[i]) + " " + playerNames[i];
			if (strlen(searchBuffer) > 0 && searchText.find(searchBuffer) == std::string::npos) continue;
			filtered.push_back(i);
		}

		std::vector<std::string> display;
		display.reserve(filtered.size());
		for (int idx : filtered) {
			int realID = playerIDs[idx];
			std::string txt = "[" + std::to_string(realID) + "] " + playerNames[idx];
			if (friendStatus[realID]) txt += XorString(" (Friend)");
			display.push_back(txt);
		}

		if (display.empty()) {
			ImGui::Text(XorString("Player not found"));
			selectedItemPlayer = -1;
			selectedPlayerID = -1;
		}
		else {
			int prevSelection = selectedItemPlayer;
			CustomImGui::SoftModernListBox(XorString("Player List"), &selectedItemPlayer, display, ImVec2(0, 330));
			if (selectedItemPlayer >= 0 && selectedItemPlayer < (int)filtered.size()) {
				selectedPlayerID = playerIDs[filtered[selectedItemPlayer]];
			}
			else {
				selectedItemPlayer = -1;
				selectedPlayerID = -1;
			}
		}

		CustomImGui::EndChild();
		ImGui::NextColumn();
		CustomImGui::Child2(XorString("RightPanel"), ImVec2(0, 0), true);

		if (selectedPlayerID >= 0) {
			int realID = selectedPlayerID;
			std::string realName = "[" + std::to_string(realID) + "] " + playerNames[selectedItemPlayer];

			ImGui::Text(XorString("Selected Player: %s"), realName.c_str());

			if (friendStatus[realID]) {
				if (CustomImGui::Button(XorString("Remove Friend"), ImVec2(300, 35)))
					friendStatus[realID] = false;
			}
			else {
				if (CustomImGui::Button(XorString("Add Friend"), ImVec2(300, 35)))
					friendStatus[realID] = true;
			}
			if (CustomImGui::Button(XorString("Teleport to"), ImVec2(300, 35))) {
				Teleport = true;
			}
		}
		else {
			ImGui::Text(XorString("No player selected"));
		}

		CustomImGui::EndChild();
		ImGui::Columns(1);
		CustomImGui::EndChild();
	}

}

void TabSettings() {
	{
		ImGui::SetCursorPosY(10);
		if (CustomImGui::SubTab(0 == Menu.SubTab7, XorString("Settings"), ImVec2(97, 30))) {
			Menu.SubTab7 = 0;
		}
	}

	ImGui::SetCursorPos(ImVec2(155, 60));
	if (Menu.SubTab7 == 0) {
		CustomImGui::Child2(XorString("Settings"), ImVec2(730, 470), true);
		{	
			CustomImGui::Checkbox(XorString("Stream Proof"), &Cheats::MenuUtils::StreamProof);
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Max Player Count"), &maxPlayerCount, 0, 5000, XorString("%d"));
			CustomImGui::SeparatorLine();
			CustomImGui::SliderInt(XorString("Overlay Delay"), &loopDelay, 0, 2000, XorString("%dms"));
			CustomImGui::SeparatorLine();
			CustomImGui::Keybind(XorString("Menu Key"), &Cheats::MenuUtils::MenuKey, 0);
			CustomImGui::SeparatorLine();
 			CustomImGui::Combo(XorString("Esp Font"), &Cheats::MenuUtils::SelectedFontIndex, Cheats::MenuUtils::Fonts, IM_ARRAYSIZE(Cheats::MenuUtils::Fonts), 9);
		}
		CustomImGui::EndChild();
	}
}

void DrawTabs() {
	ImGui::SetCursorPosY(180);
	ImGui::BeginGroup();
	{
		ImGui::BeginChild(XorString("Tabs"), ImVec2(150, 0), false);
		{
			ImGui::SetCursorPos(ImVec2(Menu.ButtonX - 7 - Menu.SeparatorOffset, 3 + 2));
			CustomImGui::Separator(XorString("Strike"));
			{
				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + Menu.ButtonSpacing));
				if (CustomImGui::Page(0 == Menu.CurrentTab, ICON_FA_PERSON_RIFLE, XorString("Aim Assist"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 0;
				}
			}

			ImGui::SetCursorPos(ImVec2(Menu.ButtonX - 7 - Menu.SeparatorOffset, 30 + Menu.ButtonHeight + Menu.ButtonSpacing + Menu.SeparatorOffset + 5));
			CustomImGui::Separator(XorString("Game"));
			{
				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 2 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(1 == Menu.CurrentTab, ICON_FA_EYE, XorString("Visual"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 1;
				}

				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 3 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(2 == Menu.CurrentTab, ICON_FA_CAR, XorString("Vehicles"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 2;
				}

				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 4 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(3 == Menu.CurrentTab, ICON_FA_GLOBE, XorString("World"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 3;
				}

				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 5 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(4 == Menu.CurrentTab, ICON_FA_BARS, XorString("Misc"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 4;
				}
			}

			ImGui::SetCursorPos(ImVec2(Menu.ButtonX - 7 - Menu.SeparatorOffset, 30 + 6 * (Menu.ButtonHeight + Menu.ButtonSpacing + 1)));
			CustomImGui::Separator(XorString("Others"));
			{
				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 7 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(5 == Menu.CurrentTab, ICON_FA_USERS, XorString("Player List"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 5;
				}
			}

			ImGui::SetCursorPos(ImVec2(Menu.ButtonX - 7 - Menu.SeparatorOffset, 30 + 8 * (Menu.ButtonHeight + Menu.ButtonSpacing + 1)));
			CustomImGui::Separator(XorString("Settings"));
			{
				ImGui::SetCursorPos(ImVec2(Menu.ButtonX, 30 + 9 * (Menu.ButtonHeight + Menu.ButtonSpacing)));
				if (CustomImGui::Page(6 == Menu.CurrentTab, ICON_FA_GEAR, XorString("Settings"), ImVec2(120, Menu.ButtonHeight))) {
					Menu.CurrentTab = 6;
				}
			}
		}
		ImGui::EndChild();

	}
	CustomImGui::EndGroup();
	ImGui::SetCursorPos(ImVec2(Menu.InitialOffsetX, 20.0f + (Menu.Spacing.y * 2.0f)));
}

void DrawBackground() {
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos, Menu.Pos + Menu.WindowSize, Menu.BackgroundColor, Menu.BackgroundRounding);
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(Menu.Pos.x, Menu.Pos.y + Menu.WindowSize.y - 30), Menu.Pos + ImVec2(Menu.WindowSize.x, Menu.WindowSize.y), ImColor(36, 39, 49), Menu.BackgroundRounding);
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(Menu.Pos.x, Menu.Pos.y + Menu.WindowSize.y - 10), Menu.Pos + ImVec2(Menu.WindowSize.x, Menu.WindowSize.y - 30), ImColor(36, 39, 49));
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos, Menu.Pos + ImVec2(150, Menu.WindowSize.y), Menu.TabColor, Menu.BackgroundRounding);
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos + ImVec2(10, 0), Menu.Pos + ImVec2(150, Menu.WindowSize.y), Menu.TabColor);
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos + ImVec2(150, 0), Menu.Pos + ImVec2(Menu.WindowSize.x - 20, 50), Menu.HeaderColor);
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos + ImVec2(200, 0), Menu.Pos + ImVec2(Menu.WindowSize.x, 50), Menu.HeaderColor, Menu.BackgroundRounding);
	ImGui::GetWindowDrawList()->AddRectFilled(Menu.Pos + ImVec2(200, 10), Menu.Pos + ImVec2(Menu.WindowSize.x, 50), Menu.HeaderColor);
	ImGui::GetWindowDrawList()->AddImage((ImTextureID)Menu.Logo, Menu.Pos + ImVec2(25, 20), Menu.Pos + ImVec2(25, 20) + ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1));
	ImGui::GetWindowDrawList()->AddText(Menu.Pos + ImVec2(23, 20) + ImVec2(-2, 95), Menu.PageTextColor, XorString("FiveM PETROV"));
	ImGui::PushFont(Menu.InterSmaller);
	ImGui::GetWindowDrawList()->AddText(Menu.Pos + ImVec2(25, 20) + ImVec2(-12, 115), Menu.PageTextColor, XorString("discord.gg/petrov"));
	static float elapsedTime = 0.0f; static int displayedFPS = 0;
	if ((elapsedTime += ImGui::GetIO().DeltaTime) >= 0.3f) {
		displayedFPS = static_cast<int>(ImGui::GetIO().Framerate);
		elapsedTime = 0.0f;
	}
	ImGui::GetWindowDrawList()->AddText(Menu.Pos + Menu.WindowSize + ImVec2(-180, -22), Menu.PageTextColor, (XorString("FPS: ") + std::to_string(displayedFPS) + XorString(" | Game Version: ") + Game.Version).c_str());
	ImGui::PopFont();
}

void DrawMenu() {
	static float fadeSpeed = 1.0f;
	if (Menu.CurrentTab != Menu.LastTab) {
		Menu.LastTab = Menu.CurrentTab;
		Menu.TabAlpha = 0.f;
	}
	if (Menu.TabAlpha < 1.0f)
	{
		Menu.TabAlpha += fadeSpeed * ImGui::GetIO().DeltaTime;
		if (Menu.TabAlpha > 1.0f)
			Menu.TabAlpha = 1.0f;
	}

	ImGui::SetNextWindowSize(Menu.WindowSize);
	ImGui::Begin(XorString("PETROV"), nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiTableFlags_NoSavedSettings);
	{
		Menu.Pos = ImGui::GetWindowPos();
		Menu.Region = ImGui::GetContentRegionMax();
		Menu.Spacing = ImGui::GetStyle().ItemSpacing;
		Menu.InitialOffsetX = Menu.Spacing.x + 145.0f;
		DrawBackground();
		DrawTabs();
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Menu.TabAlpha);
		switch (Menu.CurrentTab) {
		case 0:
			TabAimAssist();
			break;
		case 1:
			TabPlayers();
			break;
		case 2:
			TabVehicles();
			break;
		case 3:
			TabWorld();
			break;
		case 4:
			TabMisc();
			break;
		case 5:
			TabPlayerList();
			break;
		case 6:
			TabSettings();
			break;
		}
		ImGui::PopStyleVar();

	}
	ImGui::End();
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Menu.TabAlpha);

	if (Menu.CurrentTab == 0) {
		ImGui::SetNextWindowPos(ImVec2(Menu.Pos.x + Menu.WindowSize.x + 10, Menu.Pos.y + 70));
		ImGui::Begin(XorString("Hitbox Select"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		{
			CustomImGui::Child(XorString("Hitbox Select"), ImVec2(265, 490), true);
			{
				ImGui::SetCursorPosY(30);
				if (Cheats::AimAssist::SelectedMode == 0) {
					SelectTarget(Cheats::AimAssist::Aimbot::SelectedType);
				}
				else if (Cheats::AimAssist::SelectedMode == 1) {
					SelectTarget(Cheats::AimAssist::Silent::SelectedType);
				}
				else if (Cheats::AimAssist::SelectedMode == 2) {
					SelectTarget(Cheats::AimAssist::Triggerbot::SelectedType);
				}
				ImGui::SetCursorPos(ImVec2(1, 415));
				CustomImGui::RadialButtonSelector(XorString(""), &Cheats::AimAssist::SelectedMode, Cheats::AimAssist::ModesAimAssist, IM_ARRAYSIZE(Cheats::AimAssist::ModesAimAssist));
			}
			CustomImGui::EndChild();
		}
		ImGui::End();
	}

	if (Menu.CurrentTab == 1 && Menu.SubTab2 == 0) {
		ImGui::SetNextWindowPos(ImVec2(Menu.Pos.x + Menu.WindowSize.x + 10, Menu.Pos.y + 70));
		ImGui::Begin(XorString("Preview"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		{
			CustomImGui::Child(XorString("Preview"), ImVec2(265, 500), true);
			{
				ImGui::SetCursorPosY(30);
				DrawPreview();
			}
			CustomImGui::EndChild();
		}
		ImGui::End();
	}
	ImGui::PopStyleVar();
}