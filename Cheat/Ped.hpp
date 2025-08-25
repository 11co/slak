struct Bones {
	Vector3 head;
	char padding0[0x4]{};
	Vector3 leftFoot;
	char padding1[0x4]{};
	Vector3 rightFoot;
	char padding2[0x4]{};
	Vector3 leftAnkle;
	char padding3[0x4]{};
	Vector3 rightAnkle;
	char padding4[0x4]{};
	Vector3 leftHand;
	char padding5[0x4]{};
	Vector3 rightHand;
	char padding6[0x4]{};
	Vector3 neck;
	char padding7[0x4]{};
	Vector3 hip;
};

enum Bone : int {
	Head,
	LeftFoot,
	RightFoot,
	LeftAnkle,
	RightAnkle,
	LeftHand,
	RightHand,
	Neck,
	Hip
};

class Ped {
public:
	uintptr_t pointer;
	uintptr_t playerInfo;
	uintptr_t weaponManager;
	uintptr_t currentWeapon;

	float armor;
	float health;
	Vector3 position;
	Matrix boneMatrix;
	Vector3 boneList[9]{};

	bool GetPlayer(uintptr_t& base) {
		pointer = base;
		return pointer != NULL;
	}

	std::string GetWeaponName() {
		uintptr_t weaponManager = ReadMemory<uintptr_t>(pointer + Offsets.WeaponManager);
		uintptr_t step1 = ReadMemory<uintptr_t>(weaponManager + 0x20);
		return ReadString(ReadMemory<uintptr_t>(step1 + 0x5F0));
	}

	uintptr_t GetWeapon() {
		return ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(pointer + Offsets.WeaponManager) + 0x20) + 0x10);
	}

	int GetId() {
		return ReadMemory<int>(ReadMemory<uint64_t>(pointer + Offsets.PlayerInfo) + Offsets.Id);
	}

	bool IsPlayer() {
		return playerInfo != NULL;
	}

	bool IsDead() {
		return position == Vector3(0.f, 0.f, 0.f);
	}

	bool IsVisible() {
		enum VisibilityFlags : BYTE {
			InvisibleFlag1 = 36,
			InvisibleFlag2 = 0,
			InvisibleFlag3 = 4,
			IntersectMissionEntityAndTrain = 2,
			IntersectPeds1 = 4,
			IntersectVehicles = 10,
			IntersectVegetation = 256,
			FrustumCulling = 512,
			OcclusionCulling = 1024,
			DistanceCulling = 2048
		};

		BYTE VisibilityFlag = ReadMemory<BYTE>(pointer + Offsets.VisibleFlag);
		if (VisibilityFlag == InvisibleFlag1 ||
			VisibilityFlag == InvisibleFlag2 ||
			VisibilityFlag == InvisibleFlag3 ||
			VisibilityFlag == IntersectMissionEntityAndTrain ||
			VisibilityFlag == IntersectPeds1 ||
			VisibilityFlag == IntersectVehicles ||
			VisibilityFlag == IntersectVegetation ||
			(VisibilityFlag & FrustumCulling) ||
			(VisibilityFlag & OcclusionCulling) ||
			(VisibilityFlag & DistanceCulling))
		{
			return false;
		}

		return true;
	}

	bool Update() {
		playerInfo = ReadMemory<uintptr_t>(pointer + Offsets.PlayerInfo);
		currentWeapon = GetWeapon();
		health = ReadMemory<float>(pointer + Offsets.Health);
		position = ReadMemory<Vector3>(pointer + 0x90);
		armor = ReadMemory<float>(pointer + Offsets.Armor);
		boneMatrix = ReadMemory<Matrix>(pointer + 0x60);
		UpdateBones();
		return true;
	}

	void UpdateBones() {
		Bones bones = ReadMemory<Bones>(pointer + Offsets.BoneList);
		boneList[Head] = Vec3Transform(&bones.head, &boneMatrix);
		boneList[LeftFoot] = Vec3Transform(&bones.leftFoot, &boneMatrix);
		boneList[RightFoot] = Vec3Transform(&bones.rightFoot, &boneMatrix);
		boneList[LeftAnkle] = Vec3Transform(&bones.leftAnkle, &boneMatrix);
		boneList[RightAnkle] = Vec3Transform(&bones.rightAnkle, &boneMatrix);
		boneList[LeftHand] = Vec3Transform(&bones.leftHand, &boneMatrix);
		boneList[RightHand] = Vec3Transform(&bones.rightHand, &boneMatrix);
		boneList[Neck] = Vec3Transform(&bones.neck, &boneMatrix);
		boneList[Hip] = Vec3Transform(&bones.hip, &boneMatrix);
	}
};