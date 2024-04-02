#pragma once
#include <utility>
class IPiece
{
public:
	enum class PlayerColor
	{
		NoColor,
		Red,
		Blue
	};

	IPiece();
	IPiece(PlayerColor color);
	IPiece& operator=(const IPiece& copy) = default;
	IPiece(const IPiece& copy) = default;
	virtual ~IPiece() = default;

	virtual PlayerColor GetColor() const = 0;
	virtual void SetColor(const PlayerColor& color) = 0;

protected:
	PlayerColor m_color;
};