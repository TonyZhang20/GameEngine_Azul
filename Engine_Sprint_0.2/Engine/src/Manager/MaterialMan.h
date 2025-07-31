#ifndef MATERIAL_MAN_H
#define MATERIAL_MAN_H

#include "Material.h"
#include "ManBase.h"
#include "ShaderObject.h"
#include "TextureObject.h"

namespace Azul
{
	class MaterialMan : public ManBase
	{

	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static Material* Add(Material::Name name, ShaderObject* shader, TextureObject* texture = nullptr);
		static Material* Find(Material::Name name);

		static void Remove(Material* pNode);
		static void Dump();
		static Iterator* GetActiveIterator();

	protected:
		DLink* derivedCreateNode() override;

	private:
		static MaterialMan* privGetInstance();

		~MaterialMan();
		MaterialMan(int reserveNum, int reserveGrow);

	private:
		Material* poNodeCompare;
		static MaterialMan* posInstance;

	};
}

#endif // !MATERIAL_MAN_H
