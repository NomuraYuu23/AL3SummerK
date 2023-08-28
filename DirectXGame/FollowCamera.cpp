#include "FollowCamera.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <numbers>

void FollowCamera::Initialize() {

	input_ = Input::GetInstance();

	viewProjection_.Initialize();
}

void FollowCamera::Update() {

	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 0.0f, -30.0f};

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = calc->MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateMatrixY = calc->MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 rotateMatrixZ = calc->MakeRotateZMatrix(viewProjection_.rotation_.z);

		Matrix4x4 rotateMatrix =
		    calc->Multiply(rotateMatrixX, calc->Multiply(rotateMatrixY, rotateMatrixZ));

		// オフセットをカメラの回転に合わせて回転させる
		offset = calc->TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ =
		    Vector3Calc::GetInstance()->Add(target_->translation_, offset);
	}

	const float RotateSpeed = 0.01f;

	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection_.rotation_.y += RotateSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		viewProjection_.rotation_.y -= RotateSpeed;
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// 回転x上限
	float rotationLimitX = float(std::numbers::pi) / 4.0f;

	if (input_->GetJoystickState(0, joyState)) {
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * RotateSpeed;
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * RotateSpeed;
		if (viewProjection_.rotation_.x < -rotationLimitX) {
			viewProjection_.rotation_.x = -rotationLimitX; 
		}
		if (viewProjection_.rotation_.x > rotationLimitX) {
			viewProjection_.rotation_.x = rotationLimitX;
		}
	}
	// ビュー更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Reset() {

	viewProjection_.translation_ = {0, 0, 0};
	viewProjection_.rotation_ = {0, 0, 0};

}
