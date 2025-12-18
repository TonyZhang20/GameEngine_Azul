#include "Mat4Hint.h"


namespace Azul
{
	Mat4Hint::Hint Mat4Hint::GetMatHint(const Mat4& mat)
	{
		return ((Mat4Hint&)mat).GetHint();
	}

	void Mat4Hint::ClearHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privClearHint();
	}

	void Mat4Hint::SetNewHint(Mat4& changer, const Mat4& A, const Mat4& B)
	{
		((Mat4Hint&)changer).privSetNewHint(((Mat4Hint&)A).GetHint(), ((Mat4Hint&)B).GetHint());
	}

	void Mat4Hint::SetNewHint(Mat4& charger, const Hint A, const Hint B)
	{
		((Mat4Hint&)charger).privSetNewHint(A, B);
	}

	void Mat4Hint::SetGeneralHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privSetGeneralHint();
	}

	void Mat4Hint::SetRotScaleHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privSetRotScaleHint();
	}

	void Mat4Hint::SetHint(Mat4& mat, Hint h)
	{
		((Mat4Hint&)mat).privSetHint(h);
	}

	void Mat4Hint::SetRotHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privSetRotHint();
	}

	void Mat4Hint::SetTransHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privSetTransHint();
	}

	void Mat4Hint::SetScaleHint(Mat4& mat)
	{
		((Mat4Hint&)mat).privSetScaleHint();
	}

}
