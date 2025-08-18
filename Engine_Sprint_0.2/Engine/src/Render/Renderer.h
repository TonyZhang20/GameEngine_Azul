#ifndef RENDERER_H
#define RENDERER_H

namespace Azul
{

	class Scene;

	class Renderer
	{
	public:
		Renderer(Scene* scene);

	private:
		Scene* scene;
	};
}

#endif // !3DRENDERER_H