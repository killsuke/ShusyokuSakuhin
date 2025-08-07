#pragma once

enum CameraPattern {
	CAMERA_NONE,		// 何もしない
	CHASE,		// プレイヤーを追従する
	CHASE_X,		// プレイヤーをX軸方向に追従する
	CHASE_Y,		// プレイヤーをY軸方向に追従する
	SPRING_CHASE,		// 部屋から部屋への移動

	CAMERA_MAX,
};