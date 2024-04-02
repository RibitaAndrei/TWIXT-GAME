#pragma once
#include <utility>
#include "Pilon.h"

class Bridge : public IPiece
{
public:
	using Pilons = std::pair <IPiece*, IPiece*>;

	Bridge(IPiece* firstPilon, IPiece* secondPilon, IPiece::PlayerColor color);
	Bridge(Pilons pilons, IPiece::PlayerColor color);
	Bridge(const Bridge& other);
	Bridge& operator=(const Bridge& copy) = default;
	~Bridge() = default;

	IPiece* GetFirstPilon() const;
	IPiece* GetSecondPilon() const;

	void SetFirstPilon(Pilon* pilon);
	void SetSecondPilon(Pilon* pilon);

	virtual IPiece::PlayerColor GetColor() const override;
	virtual void SetColor(const IPiece::PlayerColor& color) override;

private:
	Pilons m_pilons;
};
