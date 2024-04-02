#include "Bridge.h"


Bridge::Bridge(IPiece* firstPilon, IPiece* secondPilon, IPiece::PlayerColor color)
	:m_pilons{ std::make_pair(firstPilon, secondPilon) },
	IPiece(color)
{
}

Bridge::Bridge(Bridge::Pilons pilons, IPiece::PlayerColor color)
	: m_pilons{ pilons },
	IPiece(color)
{
}

Bridge::Bridge(const Bridge& other)
	: m_pilons{ other.m_pilons }
{
}

IPiece* Bridge::GetFirstPilon() const
{
	return m_pilons.first;
}

IPiece* Bridge::GetSecondPilon() const
{
	return m_pilons.second;
}

void Bridge::SetFirstPilon(Pilon* pilon)
{
	m_pilons.first = pilon;
}

void Bridge::SetSecondPilon(Pilon* pilon)
{
	m_pilons.second = pilon;
}

IPiece::PlayerColor Bridge::GetColor() const
{
	return m_color;
}

void Bridge::SetColor(const IPiece::PlayerColor& color)
{
	m_color = color;
}

