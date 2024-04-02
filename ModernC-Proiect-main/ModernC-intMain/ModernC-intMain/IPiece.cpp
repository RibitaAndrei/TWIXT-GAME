#include "IPiece.h"

IPiece::IPiece()
	:m_color{ PlayerColor::NoColor }
{
}

IPiece::IPiece(PlayerColor color)
	:m_color{ color }
{
}