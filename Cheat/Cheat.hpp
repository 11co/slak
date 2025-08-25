#include "GameSDK.hpp"
#include "Ped.hpp"

Ped LocalPlayer;
uintptr_t TPModelInfo = NULL;
uintptr_t TPNavigation = NULL;
Vector3 TPPosition = Vector3(0, 0, 0);
std::vector<Ped> PedList;
Vector3 EndBulletPos;

struct PedBarFix {
	int id;
	float health;
	float armor;
};

std::vector<PedBarFix> pedBarFix;
inline std::string GetPedName(Ped& ped) {
	std::string pedName = XorString("Player");
	if (!Offsets.CitizenPlayernamesBase) {
		return pedName;
	}

	uint64_t playerNames = ReadMemory<uint64_t>(Offsets.CitizenPlayernamesBase + 0x30D38);
	if (!playerNames) {
		return pedName;
	}

	int playerId = ped.GetId();
	uintptr_t list = ReadMemory<uintptr_t>(playerNames + 0x8);
	for (uint32_t i = 0; i < maxPlayerCount; ++i) {
		if (!list) {
			break;
		}

		if (playerId == ReadMemory<int>(list + 0x10)) {
			pedName = ReadString(list + 0x18);
			break;
		}
		list = ReadMemory<uintptr_t>(list + 0x8);
	}
	return pedName;
}

void UpdatePeds() {
	while (!exitLoop) {
		std::vector<Ped> updatedPedList;
		Game.World = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
		LocalPlayer.pointer = ReadMemory<uintptr_t>(Game.World + Offsets.LocalPlayer);
		Game.ViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
		Game.ReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);

		uintptr_t entityListPtr = ReadMemory<uintptr_t>(Game.ReplayInterface + 0x18);
		uintptr_t entityList = ReadMemory<uintptr_t>(entityListPtr + 0x100);

		for (int i = 0; i < maxPlayerCount; i++) {
			Ped ped;
			uintptr_t player = ReadMemory<uintptr_t>(entityList + (i * 0x10));
			if (player == LocalPlayer.pointer) {
				continue;
			}
			else if (!ped.GetPlayer(player)) {
				continue;
			}
			else if (!ped.Update()) {
				continue;
			}
			updatedPedList.push_back(ped);
		}
		PedList = updatedPedList;
		Sleep(loopDelay);
	}
}

namespace Draw {
	void Always() {
		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		if (Cheats::AimAssist::Aimbot::DrawFov) {
			drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), Cheats::AimAssist::Aimbot::Fov, Cheats::AimAssist::Aimbot::Color, 100, 1.0f);
		}

		if (Cheats::AimAssist::Silent::DrawFov) {
			drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), Cheats::AimAssist::Silent::Fov, Cheats::AimAssist::Silent::Color, 100, 1.0f);
		}

		if (Cheats::AimAssist::Triggerbot::DrawFov) {
			drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), Cheats::AimAssist::Triggerbot::Fov, Cheats::AimAssist::Triggerbot::Color, 100, 1.0f);
		}
	}

	void Esp() {
		ImFont* pFont = Menu.InterSemiBold;
		if (Cheats::MenuUtils::SelectedFontIndex == 1) pFont = Menu.Arial;
		else if (Cheats::MenuUtils::SelectedFontIndex == 2) pFont = Menu.Tahoma;
		else if (Cheats::MenuUtils::SelectedFontIndex == 3) pFont = Menu.Verdana;
		else if (Cheats::MenuUtils::SelectedFontIndex == 4) pFont = Menu.SegoeUI;
		else if (Cheats::MenuUtils::SelectedFontIndex == 5) pFont = Menu.TimesNewRoman;
		else if (Cheats::MenuUtils::SelectedFontIndex == 6) pFont = Menu.Calibri;
		else if (Cheats::MenuUtils::SelectedFontIndex == 7) pFont = Menu.CourierNew;
		else if (Cheats::MenuUtils::SelectedFontIndex == 8) pFont = Menu.Consolas;
		ImGui::PushFont(pFont);
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		Matrix viewMatrix = ReadMemory<Matrix>(Game.ViewPort + 0x24C);
		if (!LocalPlayer.Update()) {
			return;
		}

		for (auto& ped : PedList) {
			if (!ped.Update()) {
				continue;
			}

			float pDistance = GetDistance(ped.position, LocalPlayer.position);
			if (pDistance >= Cheats::Players::Distance) {
				continue;
			}
			if (Cheats::Players::IgnorePed && !ped.IsPlayer()) {
				continue;
			}

			if (Cheats::Players::IgnoreDeath && ped.IsDead()) {
				continue;
			}

			if (Cheats::Players::OnlyVisible && !ped.IsVisible()) {
				continue;
			}

			int pedID = ped.GetId();
			Vector2 pBase{}, pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{};
			if (!WorldToScreen(viewMatrix, ped.position, pBase) ||
				!WorldToScreen(viewMatrix, ped.boneList[Head], pHead) ||
				!WorldToScreen(viewMatrix, ped.boneList[Neck], pNeck) ||
				!WorldToScreen(viewMatrix, ped.boneList[LeftFoot], pLeftFoot) ||
				!WorldToScreen(viewMatrix, ped.boneList[RightFoot], pRightFoot)) {
				continue;
			}

			float HeadToNeck = pNeck.y - pHead.y;
			float pTop = pHead.y - (HeadToNeck * 2.5f);
			float pBottom = (pLeftFoot.y > pRightFoot.y ? pLeftFoot.y : pRightFoot.y) * 1.001f;
			float pHeight = pBottom - pTop;
			float pWidth = pHeight / 3.5f;
			float bScale = pWidth / 1.5f;
			float reducedWidth = pWidth * Cheats::Players::DrawBox::Size;
			ImVec2 pos(pBase.x, pBottom + 2.0f);
			float spacing = 2.0f;
			float baseSize = 15.0f;
			float scaledSize = baseSize;
			if (pDistance > 100.0f) {
				scaledSize = baseSize * (100.0f / pDistance);
				if (scaledSize < 8.0f) {
					scaledSize = 8.0f;
				}
			}

			if (friendStatus.find(pedID) != friendStatus.end() && friendStatus[pedID]) {
				ImColor friendsColor = ImColor(255, 255, 0);
				DrawOutlinedText(drawList, pFont, XorString("Friend"), ImVec2(pBase.x, pTop - 15) + ImVec2(1, 1), 15.0f, ImColor(0, 0, 0, 150), true);
				DrawOutlinedText(drawList, pFont, XorString("Friend"), ImVec2(pBase.x, pTop - 15), 15.0f, friendsColor, true);
				continue;
			}

			if (Cheats::Players::DrawSkeleton::Enabled) {
				ImColor color = Cheats::Players::DrawSkeleton::Color;
				if (!ped.IsVisible()) {
					color = ImColor(255, 255, 255);
				}

				Vector2 screenHeadPos;
				for (int j = 0; j < 5; j++) {
					Vector3 skeletonList[][2] = {
						{ ped.boneList[Neck], ped.boneList[Hip] },
						{ ped.boneList[Neck], ped.boneList[LeftHand] },
						{ ped.boneList[Neck], ped.boneList[RightHand] },
						{ ped.boneList[Hip], ped.boneList[LeftFoot] },
						{ ped.boneList[Hip], ped.boneList[RightFoot] }
					};

					Vector2 ScreenB1, ScreenB2;
					if (Vec3Empty(skeletonList[j][0]) || Vec3Empty(skeletonList[j][1])) {
						break;
					}
					else if (!WorldToScreen(viewMatrix, skeletonList[j][0], ScreenB1) || !WorldToScreen(viewMatrix, skeletonList[j][1], ScreenB2)) {
						break;
					}
					DrawLine(drawList, ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), ImColor(0, 0, 0, 255), 3);
					DrawLine(drawList, ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), color, 1);
				}
			}

			if (Cheats::Players::DrawBox::Enabled) {
				ImColor color = Cheats::Players::DrawBox::Color;
				ImVec4 mainColor = color.Value;

				auto DrawBoxWithGradient = [&](bool isCornerBox) {
					if (Cheats::Players::DrawBox::GradientEnabled) {
						ImColor topColor, bottomColor;
						if (Cheats::Players::DrawBox::UseCustomGradient) {
							topColor = Cheats::Players::DrawBox::BoxGradientTopColor;
							topColor.Value.w *= Cheats::Players::DrawBox::GradientIntensity;
							bottomColor = Cheats::Players::DrawBox::BoxGradientBottomColor;
							bottomColor.Value.w *= Cheats::Players::DrawBox::GradientIntensity;
						}
						else {
							float intensity = Cheats::Players::DrawBox::GradientIntensity;
							topColor = ImColor(mainColor.x * 0.6f, mainColor.y * 0.6f, mainColor.z * 0.6f, 0.25f * intensity);
							bottomColor = ImColor(mainColor.x * 0.1f, mainColor.y * 0.1f, mainColor.z * 0.1f, 0.35f * intensity);
						}

						drawList->AddRectFilledMultiColor(ImVec2(pBase.x - reducedWidth + 1, pTop + 1), ImVec2(pBase.x + reducedWidth - 1, pBottom - 1), ImGui::ColorConvertFloat4ToU32(topColor), ImGui::ColorConvertFloat4ToU32(topColor), ImGui::ColorConvertFloat4ToU32(bottomColor), ImGui::ColorConvertFloat4ToU32(bottomColor));
					}

					if (!isCornerBox) {
						DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pTop), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x - reducedWidth, pBottom), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x + reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pBottom), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x - reducedWidth, pBottom), ImVec2(pBase.x + reducedWidth, pBottom), color, 1.f);
					}
					else {
						DrawLineOutline(ImVec2((pBase.x - reducedWidth), pTop), ImVec2((pBase.x - reducedWidth) + bScale, pTop), color, 1.f);
						DrawLineOutline(ImVec2((pBase.x + reducedWidth), pTop), ImVec2((pBase.x + reducedWidth) - bScale, pTop), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x - reducedWidth, pTop + bScale), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x - reducedWidth, pBottom), ImVec2(pBase.x - reducedWidth, pBottom - bScale), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x + reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pTop + bScale), color, 1.f);
						DrawLineOutline(ImVec2(pBase.x + reducedWidth, pBottom), ImVec2(pBase.x + reducedWidth, pBottom - bScale), color, 1.f);
						DrawLineOutline(ImVec2((pBase.x - reducedWidth), pBottom), ImVec2((pBase.x - reducedWidth) + bScale, pBottom), color, 1.f);
						DrawLineOutline(ImVec2((pBase.x + reducedWidth), pBottom), ImVec2((pBase.x + reducedWidth) - bScale, pBottom), color, 1.f);
					}
					};

				if (Cheats::Players::DrawBox::SelectedType == 0) {
					DrawBoxWithGradient(false);
				}
				else if (Cheats::Players::DrawBox::SelectedType == 1) {
					DrawBoxWithGradient(true);
				}
			}

			if (Cheats::Players::DrawLine::Enabled) {
				ImColor color = Cheats::Players::DrawLine::Color;
				ImVec2 startPos, endPos;
				ImGuiIO& io = ImGui::GetIO();
				if (Cheats::Players::DrawLine::SelectedType == 0) {
					startPos.x = pBase.x + reducedWidth;
					startPos.y = pTop;
					endPos.x = Game.lpRect.right / 2.f;
					endPos.y = 0;
				}
				if (Cheats::Players::DrawLine::SelectedType == 1) {
					startPos.x = pBase.x + reducedWidth;
					startPos.y = pTop;
					endPos.x = io.DisplaySize.x / 2.0f;
					endPos.y = io.DisplaySize.y / 2.0f;
				}
				if (Cheats::Players::DrawLine::SelectedType == 2) {
					startPos.x = pBase.x;
					startPos.y = pBottom;
					endPos.x = io.DisplaySize.x / 2.0f;
					endPos.y = io.DisplaySize.y;
				}
				DrawLineOutline(startPos, endPos, color, 1.f);
			}

			if (Cheats::Players::DrawId::Enabled) {
				ImColor color = Cheats::Players::DrawId::Color;
				if (!ped.IsVisible()) {
					color = DarkenColor(color, 0.3f);
				}

				std::string pedId = std::to_string(pedID);
				if (!pedId.empty()) {
					ImVec2 idPos;
					idPos.x = pBase.x;
					idPos.y = pTop - 27.0f;
					std::string displayText = pedId;
					DrawOutlinedText(drawList, pFont, displayText, idPos + ImVec2(1, 1), scaledSize, ImColor(0, 0, 0, 150), true);
					DrawOutlinedText(drawList, pFont, displayText, idPos, scaledSize, color, true);
				}
			}

			if (Cheats::Players::DrawName::Enabled) {
				ImColor color = Cheats::Players::DrawName::Color;
				if (!ped.IsVisible()) {
					color = DarkenColor(color, 0.3f);
				}

				std::string pedName = GetPedName(ped);
				if (!ped.IsPlayer()) {
					pedName = XorString("NPC");
				}

				if (!pedName.empty()) {
					ImVec2 idPos;
					idPos.x = pBase.x;
					idPos.y = pTop - 15.0f;
					DrawOutlinedText(drawList, pFont, pedName, idPos + ImVec2(1, 1), scaledSize, ImColor(0, 0, 0, 150), true);
					DrawOutlinedText(drawList, pFont, pedName, idPos, scaledSize, color, true);
				}
			}

			if (Cheats::Players::DrawHealth::Enabled || Cheats::Players::DrawArmor::Enabled) {
				PedBarFix fix;
				fix.id = pedID;
				fix.health = ped.health;
				fix.armor = ped.armor;
				bool exists = false;
				for (auto& item : pedBarFix) {
					if (item.id == pedID) {
						if (ped.health > 0) {
							item.health = ped.health;
						}
						else {
							ped.health = item.health;
						}

						if (ped.armor > 0) {
							item.health = ped.armor;
						}
						else {
							ped.armor = item.armor;
						}
						exists = true;
						break;
					}
				}
				if (!exists) {
					pedBarFix.push_back(fix);
				}
			}

			if (Cheats::Players::DrawHealth::Enabled) {
				float healthRatio = (float(ped.health) - 100.0f) / 100.0f;
				if (healthRatio < 0.0f) healthRatio = 0.0f;
				if (healthRatio > 1.0f) healthRatio = 1.0f;
				float barLeft = pBase.x - reducedWidth;
				float barRight = pBase.x + reducedWidth;
				float barWidth = barRight - barLeft;
				float barHeight = 0.0f;
				if (pDistance < 90.0f)
				{
					barHeight = 8.0f;
				}
				else
				{
					barHeight = pHeight * 0.07f;
					if (barHeight < 3.0f)  barHeight = 3.0f;
					if (barHeight > 12.0f) barHeight = 12.0f;
				}

				ImVec2 barPos(barLeft, pos.y);
				ImVec2 barEnd(barRight, pos.y + barHeight);
				ImU32 bgColor = IM_COL32(0, 0, 0, 255);
				ImU32 borderColor = IM_COL32(0, 0, 0, 255);
				ImVec4 colLow(1.0f, 0.0f, 0.0f, 1.0f);
				ImVec4 colHigh(0.0f, 1.0f, 0.0f, 1.0f);
				drawList->AddRectFilled(barPos, barEnd, bgColor);
				float filledWidth = barWidth * healthRatio;
				ImVec4 currentColor(colLow.x + (colHigh.x - colLow.x) * healthRatio, colLow.y + (colHigh.y - colLow.y) * healthRatio, colLow.z + (colHigh.z - colLow.z) * healthRatio, 1.0f);
				drawList->AddRectFilled(ImVec2(barLeft, barPos.y), ImVec2(barLeft + filledWidth, barPos.y + barHeight), ImGui::ColorConvertFloat4ToU32(currentColor));
				drawList->AddRect(barPos, barEnd, borderColor);
				pos.y += barHeight + 2.0f;
			}

			if (Cheats::Players::DrawArmor::Enabled) {
				float boxLeft = pBase.x - reducedWidth;
				float boxRight = pBase.x + reducedWidth;
				float boxTop = pTop;
				float boxBottom = pBottom;
				float offset = 3.0f;
				float barW = 0.0f;
				float barH = 0.0f;
				if (pDistance < 90.0f)
				{
					barW = 4.0f;
					barH = pBottom - pTop;
				}
				else
				{
					float barW = 2.0f;
					barH = pBottom - pTop;
				}

				ImVec2 barBottomLeft(boxLeft - offset - barW, boxBottom);
				float aPerc = (ped.armor / 100.0f) * barH;
				if (aPerc < 0.0f) aPerc = 0.0f;
				if (aPerc > barH) aPerc = barH;
				ImVec2 barTopLeft(barBottomLeft.x, barBottomLeft.y - barH);
				ImVec2 barBottomRight(barBottomLeft.x + barW, barBottomLeft.y);
				ImU32 bg = IM_COL32(0, 0, 0, 255);
				ImU32 outline = IM_COL32(0, 0, 0, 255);
				drawList->AddRectFilled(barTopLeft, barBottomRight, bg);
				drawList->AddRect(barTopLeft, barBottomRight, outline);
				ImVec4 colBlue(0.0f, 0.0f, 1.0f, 1.0f);
				ImVec4 colRed(1.0f, 0.0f, 0.0f, 1.0f);
				for (float i = 0; i < aPerc; i++)
				{
					float t = i / barH;
					ImVec4 c(colBlue.x + (colRed.x - colBlue.x) * t, colBlue.y + (colRed.y - colBlue.y) * t, colBlue.z + (colRed.z - colBlue.z) * t, 1.0f);
					drawList->AddRectFilled(ImVec2(barTopLeft.x, barBottomLeft.y - i - 1), ImVec2(barTopLeft.x + barW, barBottomLeft.y - i), ImGui::ColorConvertFloat4ToU32(c));
				}
			}

			if (Cheats::Players::DrawWeaponName::Enabled) {
				ImColor color = Cheats::Players::DrawWeaponName::Color;
				if (!ped.IsVisible()) {
					color = DarkenColor(color, 0.3f);
				}
				std::string weaponName = ped.GetWeaponName();
				if (!weaponName.empty())
				{
					ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(scaledSize, FLT_MAX, 0.0f, weaponName.c_str());
					DrawOutlinedText(drawList, pFont, weaponName.c_str(), pos + ImVec2(1, 1), scaledSize, ImColor(0, 0, 0, 150), true);
					DrawOutlinedText(drawList, pFont, weaponName.c_str(), pos, scaledSize, color, true);
					pos.y += textSize.y + 1.0f;
				}
			}

			if (Cheats::Players::DrawDistance::Enabled) {
				ImColor color = Cheats::Players::DrawDistance::Color;
				if (!ped.IsVisible()) {
					color = DarkenColor(color, 0.3f);
				}
				std::string dataText = "[" + std::to_string((int)pDistance) + "m]";

				ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(scaledSize, FLT_MAX, 0.0f, dataText.c_str());
				DrawOutlinedText(drawList, pFont, dataText, pos + ImVec2(1, 1), scaledSize, IM_COL32(0, 0, 0, 150), true);
				DrawOutlinedText(drawList, pFont, dataText, pos, scaledSize, color, true);
			}
		}

		ImGui::PopFont();
	}

	void Vehicle() {
		if (Cheats::Vehicle::Enabled) {
			ImDrawList* drawList = ImGui::GetBackgroundDrawList();
			uintptr_t vehicleInterface = ReadMemory<DWORD64>(Game.ReplayInterface + 0x10);
			uintptr_t vehicleList = ReadMemory<DWORD64>(vehicleInterface + 0x180);
			int vehicleListCount = ReadMemory<int>(vehicleInterface + 0x188);
			if (vehicleListCount > 300) {
				return;
			}
			for (int i = 0; i < vehicleListCount; ++i) {
				uintptr_t vehicle = ReadMemory<uintptr_t>(vehicleList + (i * 0x10));
				if (!vehicle) {
					continue;
				}

				float VehicleHealthh = ReadMemory<float>(vehicle + Offsets.Health);
				float vehicleMaxHealth = 1000;

				Vector3 getCordLocal = ReadMemory<Vector3>(LocalPlayer.pointer + 0x90);
				Matrix viewMatrix = ReadMemory<Matrix>(Game.ViewPort + 0x24C);
				Vector3 getCorrdinateVehicle = ReadMemory<Vector3>(vehicle + 0x90);
				Vector2 vehicleLocation;
				if (!WorldToScreen(viewMatrix, getCorrdinateVehicle, vehicleLocation)) {
					continue;
				}

				Vector3 distanceCalculation = getCordLocal - getCorrdinateVehicle;
				double Distance = sqrtf(distanceCalculation.x * distanceCalculation.x + distanceCalculation.y * distanceCalculation.y + distanceCalculation.z * distanceCalculation.z);

				if (Distance <= Cheats::Vehicle::Distance) {
					if ((int)Distance == 0 && !Cheats::Vehicle::DrawLocalVehicle) {
						continue;
					}

					if ((int)Distance >= 1 && !Cheats::Vehicle::DrawEnemyVehicle) {
						continue;
					}

					if (vehicleLocation.x == 0 || vehicleLocation.y == 0) {
						continue;
					}

					if (Cheats::Vehicle::VehicleHealth) {
						float healthPercentage = VehicleHealthh / vehicleMaxHealth;
						float barWidth = 50.0f;
						float barHeight = 5.0f;
						float filledWidth = barWidth * healthPercentage;
						ImColor healthColor = ImColor((int)(Cheats::Vehicle::HealthBarColor.Value.x * 255), (int)(Cheats::Vehicle::HealthBarColor.Value.y * 255), (int)(Cheats::Vehicle::HealthBarColor.Value.z * 255), (int)(Cheats::Vehicle::HealthBarColor.Value.w * 255));
						ImColor backgroundColor = ImColor(0, 0, 0, 255);
						ImVec2 barPosition(vehicleLocation.x - barWidth / 2, vehicleLocation.y - 20);
						ImVec2 barFilled(barPosition.x + filledWidth, barPosition.y + barHeight);
						drawList->AddRectFilled(barPosition, ImVec2(barPosition.x + barWidth, barPosition.y + barHeight), backgroundColor);
						drawList->AddRectFilled(barPosition, barFilled, healthColor);
					}

					if (Cheats::Vehicle::VehicleEspShowDistance) {
						ImFont* pFonts = Menu.InterSemiBold;
						ImGui::PushFont(pFonts);
						std::string vehicleDistanceText = std::to_string((int)Distance) + XorString(" m");
						DrawOutlinedText(drawList, pFonts, vehicleDistanceText, ImVec2(vehicleLocation.x, vehicleLocation.y + 20), 14.f, Cheats::Vehicle::DistanceColor, true);
						ImGui::PopFont();
					}

					if (Cheats::Vehicle::VehicleEspSnapline) {
						drawList->AddLine(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 0), ImVec2(vehicleLocation.x, vehicleLocation.y), Cheats::Vehicle::SnaplineColor, 1.f);
					}

					if (Cheats::Vehicle::VehicleMarker) {
						drawList->AddCircle(ImVec2(vehicleLocation.x, vehicleLocation.y + 45), 4, Cheats::Vehicle::MarkerOuterColor, 100, 3.0f);
						drawList->AddCircle(ImVec2(vehicleLocation.x, vehicleLocation.y + 45), 2, Cheats::Vehicle::MarkerInnerColor, 100, 3.0f);
					}
				}
			}
		}
	}
}

void RestoreSilent() {
	std::vector<uint8_t> ReWriteTable = { 0xF3, 0x41, 0x0F, 0x10, 0x19,0xF3, 0x41, 0x0F, 0x10, 0x41, 0x04,0xF3, 0x41, 0x0F, 0x10, 0x51, 0x08 };
	WriteBytes(Offsets.GameBase + Offsets.Silent, &ReWriteTable[0], ReWriteTable.size());
	std::vector<uint8_t> AngleReWriteTable = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00 };
	WriteBytes(Offsets.GameBase + 0x34E, &AngleReWriteTable[0], AngleReWriteTable.size());
}

void ApplySilent() {
	static uint64_t HandleBulletAddress = Offsets.GameBase + Offsets.Silent;
	static uint64_t AllocPtr = Offsets.GameBase + 0x34E;

	auto CalculateRelativeOffset = [](uint64_t CurrentAddress, uint64_t TargetAddress, int Offset = 5) {
		intptr_t RelativeOffset = static_cast<intptr_t>(TargetAddress - (CurrentAddress + Offset));
		return static_cast<uint32_t>(RelativeOffset);
		};

	union
	{
		float f;
		uint32_t i;
	} EndPosX, EndPosY, EndPosZ;

	EndPosX.f = EndBulletPos.x;
	EndPosY.f = EndBulletPos.y;
	EndPosZ.f = EndBulletPos.z;

	{
		std::vector<uint8_t> ReWriteTable =
		{
			0xE9, 0x00, 0x00, 0x00, 0x00
		};

		uint32_t JmpOffset = CalculateRelativeOffset(HandleBulletAddress, AllocPtr);
		ReWriteTable[1] = static_cast<uint8_t>(JmpOffset & 0xFF);
		ReWriteTable[2] = static_cast<uint8_t>((JmpOffset >> 8) & 0xFF);
		ReWriteTable[3] = static_cast<uint8_t>((JmpOffset >> 16) & 0xFF);
		ReWriteTable[4] = static_cast<uint8_t>((JmpOffset >> 24) & 0xFF);
		WriteBytes(HandleBulletAddress, &ReWriteTable[0], ReWriteTable.size());
	}

	{
		uintptr_t currentAddress = (uintptr_t)AllocPtr;
		uintptr_t targetAddress = (uintptr_t)(HandleBulletAddress);
		intptr_t relativeOffset = static_cast<intptr_t>(targetAddress - (currentAddress + 28));
		uint32_t jmpOffset = static_cast<uint32_t>(relativeOffset);

		std::vector<uint8_t> ReWriteTable =
		{
			0x41, 0xC7, 0x01, static_cast<uint8_t>(EndPosX.i), static_cast<uint8_t>(EndPosX.i >> 8), static_cast<uint8_t>(EndPosX.i >> 16), static_cast<uint8_t>(EndPosX.i >> 24),
			0x41, 0xC7, 0x41, 0x04, static_cast<uint8_t>(EndPosY.i), static_cast<uint8_t>(EndPosY.i >> 8), static_cast<uint8_t>(EndPosY.i >> 16), static_cast<uint8_t>(EndPosY.i >> 24),
			0x41, 0xC7, 0x41, 0x08, static_cast<uint8_t>(EndPosZ.i), static_cast<uint8_t>(EndPosZ.i >> 8), static_cast<uint8_t>(EndPosZ.i >> 16), static_cast<uint8_t>(EndPosZ.i >> 24),
			0xF3, 0x41, 0x0F, 0x10, 0x19,
			0xE9, 0x00, 0x00, 0x00, 0x00
		};

		ReWriteTable[29] = static_cast<uint8_t>(jmpOffset & 0xFF);
		ReWriteTable[30] = static_cast<uint8_t>((jmpOffset >> 8) & 0xFF);
		ReWriteTable[31] = static_cast<uint8_t>((jmpOffset >> 16) & 0xFF);
		ReWriteTable[32] = static_cast<uint8_t>((jmpOffset >> 24) & 0xFF);
		WriteBytes(AllocPtr, &ReWriteTable[0], ReWriteTable.size());
	}
}

Ped FindBestTarget(int aimFov) {
	Ped bestTarget;
	float minFov = 9999.f;
	for (auto& ped : PedList) {
		if (!LocalPlayer.Update()) {
			break;
		}

		if (!ped.Update()) {
			continue;
		}

		float pDistance = GetDistance(ped.position, LocalPlayer.position);
		if (pDistance >= Cheats::AimAssist::Aimbot::Distance) {
			continue;
		}

		if (Cheats::AimAssist::OnlyVisible && !ped.IsVisible()) {
			continue;
		}

		if (Cheats::AimAssist::IgnorePed && !ped.IsPlayer()) {
			continue;
		}

		if (Cheats::AimAssist::IgnoreDeath && ped.IsDead()) {
			continue;
		}

		int pedID = ped.GetId();
		if (friendStatus.find(pedID) != friendStatus.end() && friendStatus[pedID]) {
			continue;
		}

		Vector2 screenPosition;
		Matrix viewMatrix = ReadMemory<Matrix>(Game.ViewPort + 0x24C);
		if (Cheats::AimAssist::Aimbot::SelectedType == 0) {
			if (!WorldToScreen(viewMatrix, ped.boneList[Head], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 1) {
			if (!WorldToScreen(viewMatrix, ped.boneList[Neck], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 2) {
			if (!WorldToScreen(viewMatrix, ped.boneList[Hip], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 3) {
			if (!WorldToScreen(viewMatrix, ped.boneList[RightHand], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 4) {
			if (!WorldToScreen(viewMatrix, ped.boneList[LeftHand], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 5) {
			if (!WorldToScreen(viewMatrix, ped.boneList[RightAnkle], screenPosition)) {
				continue;
			}
		}
		else if (Cheats::AimAssist::Aimbot::SelectedType == 6) {
			if (!WorldToScreen(viewMatrix, ped.boneList[LeftAnkle], screenPosition)) {
				continue;
			}
		}
		float fov = abs((Vector2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f) - screenPosition).Length());
		if (fov < aimFov) {
			if (fov < minFov) {
				bestTarget = ped;
				minFov = fov;
				continue;
			}
		}
	}
	return bestTarget;
}

void SetAim() {
	while (!exitLoop) {
		if (Cheats::AimAssist::Aimbot::Enabled) {
			Ped target = FindBestTarget(Cheats::AimAssist::Aimbot::Fov);
			if (GetAsyncKeyState(Cheats::AimAssist::Aimbot::Key) & 0x8000) {
				if (!Vec3Empty(target.boneList[Head])) {
					uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
					Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
					Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);
					Vector3 angle;

					if (Cheats::AimAssist::Aimbot::SelectedType == 0) {
						if (Vec3Empty(target.boneList[Head])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[Head]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 1) {
						if (Vec3Empty(target.boneList[Neck])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[Neck]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 2) {
						if (Vec3Empty(target.boneList[Hip])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[Hip]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 3) {
						if (Vec3Empty(target.boneList[RightHand])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[RightHand]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 4) {
						if (Vec3Empty(target.boneList[LeftHand])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[LeftHand]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 5) {

						if (Vec3Empty(target.boneList[RightAnkle])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[RightAnkle]);
					}
					else if (Cheats::AimAssist::Aimbot::SelectedType == 6) {
						if (Vec3Empty(target.boneList[LeftAnkle])) {
							continue;
						}
						angle = CalcAngle(cameraPosition, target.boneList[LeftAnkle]);

					}
					NormalizeAngles(angle);
					Vector3 delta = angle - viewAngle;
					NormalizeAngles(delta);
					Vector3 writeAngle = viewAngle + (Cheats::AimAssist::Aimbot::Smooth ? delta / Cheats::AimAssist::Aimbot::Smooth : delta);
					NormalizeAngles(writeAngle);
					if (!Vec3Empty(writeAngle)) {
						WriteMemory<Vector3>(camera + 0x3D0, writeAngle);
					}
				}
			}
		}

		if (Cheats::AimAssist::Silent::Enabled) {
			static bool initialized = false;
			if (!initialized) {
				srand(static_cast<unsigned>(time(0)));
				initialized = true;
			}

			HANDLE x9c;
			std::vector<int> boneTypes = { Hip, Hip, Neck, Hip, Hip, Neck, Hip, Hip, Neck, Hip, Hip, Neck };
			int randomIndex = rand() % boneTypes.size();
			Ped target = FindBestTarget(Cheats::AimAssist::Silent::Fov);
			Vector3 SlientType;
			uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
			Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
			Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);
			if (Cheats::AimAssist::Silent::RandomTarget) {
				SlientType = target.boneList[boneTypes[randomIndex]];
			}
			else {
				if (Cheats::AimAssist::Silent::SelectedType == 0) {
					SlientType = target.boneList[Head];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 1) {
					SlientType = target.boneList[Neck];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 2) {
					SlientType = target.boneList[Hip];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 3) {
					SlientType = target.boneList[RightHand];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 4) {
					SlientType = target.boneList[LeftHand];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 5) {
					SlientType = target.boneList[RightAnkle];
				}
				else if (Cheats::AimAssist::Silent::SelectedType == 6) {
					SlientType = target.boneList[LeftAnkle];
				}
			}


			float distanceToTarget = GetDistance(LocalPlayer.position, target.position);
			if (distanceToTarget < 5.0f) {
				SlientType = target.boneList[boneTypes[randomIndex]];
			}
			if (Vec3Empty(SlientType)) {
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RestoreSilent, NULL, NULL, NULL);
			}
			else {
				Vector3 angle = CalcAngle(cameraPosition, SlientType);
				Vector3 delta = angle - viewAngle;
				NormalizeAngles(delta);
				float fovDistance = sqrtf(delta.x * delta.x + delta.y * delta.y);
				if (fovDistance <= Cheats::AimAssist::Silent::Fov) {
					if (GetAsyncKeyState(Cheats::AimAssist::Silent::Key) & 0x8000) {
						EndBulletPos = SlientType;
						if (Cheats::AimAssist::Silent::Pslient) {
							x9c = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RestoreSilent, NULL, 0, NULL);
							x9c = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ApplySilent, NULL, 0, NULL);
							x9c = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RestoreSilent, NULL, 0, NULL);
							CloseHandle(x9c);
							CloseHandle(x9c);
							CloseHandle(x9c);
						}
						else {
							x9c = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ApplySilent, NULL, 0, NULL);
							CloseHandle(x9c);
						}
					}
				}
				else {
					x9c = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RestoreSilent, NULL, 0, NULL);
					CloseHandle(x9c);
				}
			}
		}
		Sleep(loopDelay);
	}
}

bool IsTargetInCrosshair(const Vector2& screenPosition) {
	const float crosshairX = (float)Game.lpRect.right / 2;
	const float crosshairY = (float)Game.lpRect.bottom / 2;
	return (abs(screenPosition.x - crosshairX) <= Cheats::AimAssist::Triggerbot::CrosshairTolerance && abs(screenPosition.y - crosshairY) <= Cheats::AimAssist::Triggerbot::CrosshairTolerance);
}

void TriggerBot() {
	while (exitLoop == false) {
		if (Cheats::AimAssist::Triggerbot::Enabled && (GetAsyncKeyState(Cheats::AimAssist::Triggerbot::Key) & 0x8000)) {
			Matrix viewMatrix = ReadMemory<Matrix>(Game.ViewPort + 0x24C);
			Ped target = FindBestTarget(Cheats::AimAssist::Triggerbot::Fov);
			Vector3 TriggerBottype;
			if (Cheats::AimAssist::Triggerbot::SelectedType == 0) {
				TriggerBottype = target.boneList[Head];
			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 1) {
				TriggerBottype = target.boneList[Neck];
			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 2) {
				TriggerBottype = target.boneList[Hip];

			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 3) {
				TriggerBottype = target.boneList[RightHand];
			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 4) {
				TriggerBottype = target.boneList[LeftHand];
			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 5) {
				TriggerBottype = target.boneList[RightAnkle];
			}
			else if (Cheats::AimAssist::Triggerbot::SelectedType == 6) {
				TriggerBottype = target.boneList[LeftAnkle];
			}
			if (!Vec3Empty(TriggerBottype)) {
				Vector2 screenPosition;
				if (WorldToScreen(viewMatrix, TriggerBottype, screenPosition)) {
					if (IsTargetInCrosshair(screenPosition)) {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						Sleep(Cheats::AimAssist::Triggerbot::Delay);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
			}
		}
		Sleep(loopDelay);
	}
}

void TeleportObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3 Position, Vector3 VisualPosition, bool Stop) {
	float BackupMagic = 0.f;
	if (Stop) {
		BackupMagic = ReadMemory<float>(ModelInfo + 0x2C);
		WriteMemory(ModelInfo + 0x2C, 0.f);
	}

	WriteMemory(Object + 0x90, VisualPosition);
	WriteMemory(Navigation + 0x50, Position);
	if (Stop) {
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		WriteMemory(ModelInfo + 0x2C, BackupMagic);
	}
}

Vector3 GetPositionByID(int targetID) {
	for (auto& ped : PedList) {
		if (!ped.Update()) continue;
		if (ped.GetId() == targetID) {
			return ped.position;
		}
	}
}

void AddPlayerList() {
	while (true) {
		if (!LocalPlayer.Update()) return;

		std::vector<int> oldPlayerIDs = playerIDs;
		std::vector<int> newPlayerIDs;
		std::vector<std::string> newPlayerNames;
		newPlayerIDs.reserve(PedList.size());
		newPlayerNames.reserve(PedList.size());

		for (auto& ped : PedList) {
			if (!ped.Update()) continue;
			if (Cheats::Players::IgnorePed && !ped.IsPlayer()) continue;

			float distance = GetDistance(ped.position, LocalPlayer.position);
			if (distance > 300.0f) continue;

			int pedID = ped.GetId();
			std::string pedName = "";

			newPlayerIDs.push_back(pedID);
			newPlayerNames.push_back(pedName);

			if (friendStatus.find(pedID) == friendStatus.end()) {
				friendStatus[pedID] = false;
			}
		}

		int lastSelectedID = (selectedItemPlayer >= 0 && selectedItemPlayer < (int)playerIDs.size()) ? playerIDs[selectedItemPlayer] : -1;
		playerIDs = std::move(newPlayerIDs);
		playerNames = std::move(newPlayerNames);

		if (lastSelectedID != -1) {
			auto it = std::find(playerIDs.begin(), playerIDs.end(), lastSelectedID);
			selectedItemPlayer = (it != playerIDs.end()) ? std::distance(playerIDs.begin(), it) : 0;
		}
		else {
			selectedItemPlayer = -1;
		}

		if (Teleport && selectedItemPlayer >= 0 && selectedItemPlayer < (int)playerIDs.size()) {
			int targetID = playerIDs[selectedItemPlayer];
			Vector3 pos = GetPositionByID(targetID);
			if (!(pos.x == 0.f && pos.y == 0.f && pos.z == 0.f)) {
				uint64_t Object = (uint64_t)LocalPlayer.pointer;
				if (Object) {
					uint64_t Navigation = ReadMemory<uint64_t>(Object + 0x30);
					uint64_t ModelInfo = ReadMemory<uint64_t>(Object + 0x20);

					if (Navigation && ModelInfo) {
						Vector3 TeleportPos = Vector3(pos.x, pos.y, pos.z);
						TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);
					}
				}
			}
			Teleport = false;
		}
		Sleep(loopDelay);
	}
}

void NoClip(Vector3 CameraPos) {
	if (!LocalPlayer.pointer)
		return;

	if (!TPModelInfo)
		return;

	static float MagicValue = 0.0f;
	static bool Restoring = false;
	Vector3 NewPosition = TPPosition;

	if (!Cheats::NoClip::Enabled) {
		if (Restoring) {
			WriteMemory<float>(TPModelInfo + 0x2C, MagicValue);
		}
		Restoring = false;
		return;
	}

	if (!Restoring) {
		MagicValue = ReadMemory<float>(TPModelInfo + 0x2C);
		WriteMemory<float>(TPModelInfo + 0x2C, 0.f);
		Restoring = true;
	}

	auto Speed = static_cast<float>(Cheats::NoClip::Speed) / 100.f;

	if (GetAsyncKeyState(VK_SHIFT)) {
		Speed *= 4.f;
	}

	if (GetAsyncKeyState('W')) {
		NewPosition.x += (CameraPos * Speed).x;
		NewPosition.y += (CameraPos * Speed).y;
		NewPosition.z += (CameraPos * Speed).z;
	}

	if (GetAsyncKeyState('S')) {
		NewPosition.x -= (CameraPos * Speed).x;
		NewPosition.y -= (CameraPos * Speed).y;
		NewPosition.z -= (CameraPos * Speed).z;
	}

	Vector3 Right = CameraPos.Cross(Vector3(0, 0, 1));
	if (GetAsyncKeyState('A')) {
		NewPosition.x -= (Right * Speed).x;
		NewPosition.y -= (Right * Speed).y;
		NewPosition.z -= (Right * Speed).z;
	}

	if (GetAsyncKeyState('D')) {
		NewPosition.x += (Right * Speed).x;
		NewPosition.y += (Right * Speed).y;
		NewPosition.z += (Right * Speed).z;
	}

	if (GetAsyncKeyState(VK_SPACE)) {
		NewPosition.z += (Speed);
	}

	if (GetAsyncKeyState(VK_LCONTROL)) {
		NewPosition.z -= (Speed);
	}

	TeleportObject(LocalPlayer.pointer, TPNavigation, 0, NewPosition, NewPosition, false);
}

void MapTeleport() {
	static const char* locations[] = {
		"Legion Square", "Paleto Bay", "Main LS Customs", "IAA Roof", "FIB Roof",
		"Maze Bank", "Mount Chiliad", "Casino", "Prison", "Military Base", "Void",
		"Central Los Santos Hospital", "Pillbox Hill Medical Center", "Mount Zonah Medical Center",
		"Sandy Shores Medical Center", "Paleto Bay Medical Center",
		"Rockford Hills Medical Center", "Vinewood Medical Center", "Eclipse Medical Tower",
		"Davis Medical Center", "El Burro Heights Hospital", "Hawick Avenue Clinic",
		"Bayview Lodge Hospital", "Thomson Scrapyard Clinic", "Grapeseed Medical Clinic",
		"Torture Room X", "Trevor's Meth Lab", "Pacific Standard Bank Vault",
		"Lester's House", "Strip Club DJ Booth", "Blaine County Savings Bank",
		"Police Station", "Humane Labs Entrance", "Burnt FIB Building",
		"10 Car Garage Back Room", "Humane Labs Tunnel", "Floyd's Apartment"
	};

	static const Vector3 teleportLocations[] = {
		{ 190.52f, -873.23f, 31.5f },
		{ -138.52f, 6356.99f, 31.49f },
		{ -365.425f, -131.809f, 37.873f },
		{ 134.085f, -637.859f, 262.851f },
		{ -150.126f, -754.591f, 262.865f },
		{ -75.015f, -818.215f, 326.176f },
		{ 495.0f, 5589.0f, 795.0f },
		{ 911.957f, 38.3382f, 80.7207f },
		{ 1702.08f, 2650.51f, 45.5649f },
		{ -2751.12f, 3316.4f, 32.8117f },
		{ 15000.0f, 15000.0f, 0.0f },
		{ 339.85f, -1394.56f, 32.51f },
		{ 307.87f, -595.55f, 43.28f },
		{ -450.99f, -340.91f, 34.50f },
		{ 1839.6f, 3672.93f, 34.28f },
		{ -247.76f, 6325.57f, 32.43f },
		{ -874.51f, -307.55f, 39.58f },
		{ 639.11f, 1.12f, 82.79f },
		{ -691.28f, 293.03f, 83.16f },
		{ 360.65f, -582.14f, 28.82f },
		{ 1154.02f, -1520.58f, 34.84f },
		{ 305.24f, -1434.0f, 29.8f },
		{ -245.18f, 6229.42f, 31.49f },
		{ 2487.47f, 4960.48f, 44.37f },
		{ 1826.9f, 3685.8f, 34.27f },
		{ 147.170f, -2201.804f, 4.688f },
		{ 1391.773f, 3608.716f, 38.942f },
		{ 255.851f, 217.030f, 101.683f },
		{ 1273.898f, -1719.304f, 54.771f },
		{ 126.135f, -1278.583f, 29.270f },
		{ -109.299f, 6464.035f, 31.627f },
		{ 436.491f, -982.172f, 30.699f },
		{ 3619.749f, 2742.740f, 28.690f },
		{ 160.868f, -745.831f, 250.063f },
		{ 223.193f, -967.322f, 99.000f },
		{ 3525.495f, 3705.301f, 20.992f },
		{ -1150.703f, -1520.713f, 10.633f }
	};

	static int selected_location = -1;
	for (int i = 0; i < IM_ARRAYSIZE(locations); i++) {
		if (CustomImGui::SelectTable(locations[i], selected_location == i)) {
			selected_location = i;
		}
		if (selected_location == i && ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0)) {
			if (selected_location >= 0 && selected_location < IM_ARRAYSIZE(teleportLocations)) {
				Vector3 currentPos = ReadMemory<Vector3>(LocalPlayer.pointer + 0x90);
				uintptr_t navig = ReadMemory<uintptr_t>(LocalPlayer.pointer + 0x30);
				Vector3 targetPos = teleportLocations[selected_location];
				float distanceThreshold = 2.0f;
				float distance = sqrtf(powf(currentPos.x - targetPos.x, 2) +
					powf(currentPos.y - targetPos.y, 2) +
					powf(currentPos.z - targetPos.z, 2));

				if (distance <= distanceThreshold) {
				}
				else {
					WriteMemory<Vector3>(navig + 0x30, Vector3(0, 0, 0));
					WriteMemory<Vector3>(LocalPlayer.pointer + 0x90, targetPos);
					WriteMemory<Vector3>(navig + 0x50, targetPos);
				}
			}
		}
	}
}

void TeleportToWaypoint(bool toogle = false) {
	if (toogle) {
		if (Cheats::Teleport::posX == 0.0f && Cheats::Teleport::posY == 0.0f) {
			return;
		}
		uint64_t Object = NULL;
		uint64_t Navigation = NULL;
		uint64_t ModelInfo = NULL;
		if (!Object || !Navigation || !ModelInfo) {
			Object = (uint64_t)LocalPlayer.pointer;
			Navigation = ReadMemory<uint64_t>(LocalPlayer.pointer + 0x30);
			ModelInfo = ReadMemory<uint64_t>(LocalPlayer.pointer + 0x20);
		}
		Vector3 TeleportPos = Vector3(Cheats::Teleport::posX, Cheats::Teleport::posY, -210.f);
		TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);

	}
	else {
		for (int i = 0; i < 2000; i++) {
			uint64_t Blip = ReadMemory<uint64_t>(Offsets.GameBase + Offsets.Waypoint + (i * 8));
			if (!Blip)
				continue;
			int BlipIcon = ReadMemory<int>(Blip + 0x40);
			int BlipColor = ReadMemory<int>(Blip + 0x48);
			if ((BlipColor != 84) || (BlipIcon != 8))
				continue;
			Vector2 WaypointPos = ReadMemory<Vector2>(Blip + 0x10);
			if (WaypointPos.x != 0 && WaypointPos.y != 0) {
				uint64_t Object = NULL;
				uint64_t Navigation = NULL;
				uint64_t ModelInfo = NULL;
				if (!Object || !Navigation || !ModelInfo) {
					Object = (uint64_t)LocalPlayer.pointer;
					Navigation = ReadMemory<uint64_t>(LocalPlayer.pointer + 0x30);
					ModelInfo = ReadMemory<uint64_t>(LocalPlayer.pointer + 0x20);
				}
				Vector3 TeleportPos = Vector3(WaypointPos.x, WaypointPos.y, -210.f);
				TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);
			}
		}
	}
}

void Exploits() {
	while (!exitLoop) {
		uintptr_t weaponManager = ReadMemory<uintptr_t>(LocalPlayer.pointer + Offsets.WeaponManager);
		uintptr_t weaponinfo = ReadMemory<uintptr_t>(weaponManager + 0x20);

		TPModelInfo = ReadMemory<uintptr_t>(LocalPlayer.pointer + 0x20);
		TPPosition = ReadMemory<Vector3>(LocalPlayer.pointer + 0x90);
		TPNavigation = ReadMemory<uintptr_t>(LocalPlayer.pointer + 0x30);

		uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.BlipList);
		uintptr_t camera2 = ReadMemory<uintptr_t>(camera + 0x3C0);
		Vector3 camerapos = ReadMemory<Vector3>(camera2 + 0x40);
		NoClip(camerapos);

		if (Cheats::Exploit::HealthBoost) {
			if (GetAsyncKeyState(Cheats::Exploit::HealthBoostKey) & 0x8000) {
				WriteMemory<float>(LocalPlayer.pointer + 0x280, static_cast<float>(Cheats::Exploit::HealthBoostValue));
			}
		}

		if (Cheats::Exploit::ArmorBoost) {
			if (GetAsyncKeyState(Cheats::Exploit::ArmorBoostKey) & 0x8000) {
				float ArmorCheck = ReadMemory<float>(LocalPlayer.pointer + Offsets.Armor);
				if (ArmorCheck < 100) {
					float newArmorValue = ArmorCheck + static_cast<float>(Cheats::Exploit::ArmorBoostValue);
					if (newArmorValue > 100) {
						newArmorValue = 100;
					}
					WriteMemory<float>(LocalPlayer.pointer + Offsets.Armor, newArmorValue);
				}
			}
		}

		if (Cheats::Exploit::InfiniteAmmo) {
			uintptr_t AmmoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
			uintptr_t AmmoCount = ReadMemory<uintptr_t>(AmmoInfo + 0x8);
			uintptr_t AmmoCount2 = ReadMemory<uintptr_t>(AmmoCount + 0x0);
			WriteMemory<float>(AmmoCount2 + 0x18, 30);
		}

		if (Cheats::Exploit::NoRecoil) {
			WriteMemory<float>(weaponinfo + 0x2F4, 0.f);
		}

		if (Cheats::Exploit::NoSpread) {
			WriteMemory<float>(weaponinfo + 0x84, 0.0f);
		}

		if (Cheats::Exploit::NoReload) {
			WriteMemory<float>(weaponinfo + 0x134, 1000);
		}

		if (Cheats::Exploit::NoRange) {
			WriteMemory<float>(weaponinfo + 0x28C, 1000.f);
		}

		if (Cheats::Exploit::ReloadAmmo) {
			if (GetAsyncKeyState(Cheats::Exploit::ReloadAmmoKey) & 0x8000) {
				uintptr_t AmmoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
				uintptr_t AmmoCount = ReadMemory<uintptr_t>(AmmoInfo + 0x8);
				uintptr_t AmmoCount2 = ReadMemory<uintptr_t>(AmmoCount + 0x0);
				WriteMemory<float>(AmmoCount2 + 0x18, Cheats::Exploit::ReloadValue);
			}
		}

		if (Cheats::Teleport::TeleportWaypoint) {
			TeleportToWaypoint(false);
			Cheats::Teleport::TeleportWaypoint = false;
		}
		if (Cheats::Teleport::CustomTeleportWaypoint) {
			TeleportToWaypoint(true);
			Cheats::Teleport::CustomTeleportWaypoint = false;
		}

		if (Cheats::Vehicle::Fix) {
			if (GetAsyncKeyState(Cheats::Vehicle::FixKey) & 0x8000) {
				uintptr_t vehiclePtr = ReadMemory<uintptr_t>(LocalPlayer.pointer + Offsets.Vehicle);
				if (vehiclePtr) {
					WriteMemory<uint8_t>(vehiclePtr + 0x972, 0x17);
				}
			}
		}
		Sleep(loopDelay);
	}
}