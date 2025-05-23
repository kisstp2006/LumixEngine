#include "editor_asset.h"
#include "utils.h"

namespace Lumix {

EditorAssetPlugin::EditorAssetPlugin(const char* name, const char* ext, ResourceType type, StudioApp& app, IAllocator& allocator)
	: m_app(app)
	, m_name(name)
	, m_extension(ext)
	, m_type(type)
{
	AssetCompiler& compiler = app.getAssetCompiler();
	compiler.registerExtension(ext, type);
	const char* extensions[] = { ext };
	compiler.addPlugin(*this, Span(extensions));
	AssetBrowser& browser = app.getAssetBrowser();
	browser.addPlugin(*this, Span(extensions));
}

EditorAssetPlugin::~EditorAssetPlugin() {
	m_app.getAssetBrowser().removePlugin(*this);
	m_app.getAssetCompiler().removePlugin(*this);
}

AssetEditorWindow::AssetEditorWindow(StudioApp& app)
	: m_app(app)
{}

void AssetEditorWindow::onGUI() {
	bool open = true;

	ImGui::SetNextWindowDockID(m_dock_id ? m_dock_id : m_app.getDockspaceID(), ImGuiCond_Appearing);

	if (m_focus_request) {
		ImGui::SetNextWindowFocus();
		m_focus_request = false;
	}

	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings;
	if (m_dirty) flags |= ImGuiWindowFlags_UnsavedDocument;

	StringView basename = Path::getBasename(getPath());
	StaticString<128> title(basename, "##ae", (uintptr)this);

	if (ImGui::Begin(title, &open, flags)) {
		m_dock_id = ImGui::GetWindowDockID();
		if (m_app.checkShortcut(m_app.getCommonActions().close_window)) open = false;
		windowGUI();
	}
	if (!open) {
		if (m_dirty) {
			openCenterStrip("Confirm##cvse");
		}
		else {
			m_app.getAssetBrowser().closeWindow(*this);
		}
	}

	if (beginCenterStrip("Confirm##cvse", 6)) {
		ImGui::NewLine();		
		ImGuiEx::TextCentered("Are you sure? All changes will be lost.");
		ImGui::NewLine();
		alignGUICenter([&]{
			if (ImGui::Button("Close")) m_app.getAssetBrowser().closeWindow(*this);
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
		});
		ImGui::EndPopup();
	}
	ImGui::End();
}

}