#pragma once

namespace AmberEngine::UI
{
	class APanel
	{
	public:
		APanel();
		virtual ~APanel() = default;

		void Draw();

	protected:
		virtual void DrawImplementation() = 0;

	public:
		bool enabled = true;
	};
}
