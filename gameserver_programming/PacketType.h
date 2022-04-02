#pragma once

enum class StCPacket
{
	eLoginConfirmed,
	eCreatePiece,
	eUpdatePosition,
	eUserDisconnected,
};

enum class CtSPacket
{
	eLoginRequest,
	eUserInput,
};

