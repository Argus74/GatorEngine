#ifndef PROPERTYWINDOW_H
#define PROPERTYWINDOW_H

#include "UIWindow.h"

class PropertyWindow : public UIWindow
{
public:
    PropertyWindow();
    PropertyWindow(PropertyWindow const &) = delete;
    PropertyWindow &operator=(PropertyWindow const &) = delete;

private:
    // UIWindow virtual function implementations
    void SetPosition() override;
    void PreDraw() override;
    void DrawFrames() override;
    void PostDraw() override;

    // Draw a component section
    template <typename T>
	void DrawComponent(T& component);

    // Draw the properties of a component
    void DrawComponentProperties(std::shared_ptr<CUserInput> userinput);
    void DrawComponentProperties(std::shared_ptr<CTransform> transform);
    void DrawComponentProperties(std::shared_ptr<CName> name);
    void DrawComponentProperties(std::shared_ptr<CShape> shape);
	void DrawComponentProperties(std::shared_ptr<CSprite> sprite);
	void DrawComponentProperties(std::shared_ptr<CAnimation> animation);
	void DrawComponentProperties(std::shared_ptr<CRigidBody> rigidbody);

    // Draw one property row of a component
    template <typename T>
    void DrawProperty(const char *name, T &value);

    // Draw an input field (text box, color picker, etc.) for a property row
    // TODO: May need to update parameters, like switching to our new Vec2 object
    void DrawInputField(std::string &val);
    void DrawInputField(sf::Color &val);
    void DrawInputField(Vec2 &val);
    void DrawInputField(float &val);
    void DrawInputField(int &val);
    void DrawInputField(bool &val);
    void DrawInputField(Action &val);
    void DrawInputField(sf::Keyboard::Key &val);
    void DrawInputField(sf::Mouse::Button& val);

	// Draw a button that opens a popup for some subject
	template <typename T>
	void DrawPopupButton(const char* name, T& subject, ImVec2 size); // todo rename
	
	// Draw the actual popup for a subject
	void DrawPopup(std::shared_ptr<CUserInput> userinput);
	void DrawPopup(std::shared_ptr<Entity> entity);

    // ImGui customization options for tables (the rows under each component header)
    ImGuiTableFlags table_flags;

    // ImGui customization options for tree nodes (the component header bars)
    ImGuiTreeNodeFlags tree_node_flags;
};

// Used to map SFML key enums to strings for display in the property window
static const char* kSFMLKeyNames[] = {
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Num0",
	"Num1",
	"Num2",
	"Num3",
	"Num4",
	"Num5",
	"Num6",
	"Num7",
	"Num8",
	"Num9",
	"Escape",
	"LControl",
	"LShift",
	"LAlt",
	"LSystem",
	"RControl",
	"RShift",
	"RAlt",
	"RSystem",
	"Menu",
	"LBracket",
	"RBracket",
	"SemiColon",
	"Comma",
	"Period",
	"Quote",
	"Slash",
	"BackSlash",
	"Tilde",
	"Equal",
	"Dash",
	"Space",
	"Return",
	"BackSpace",
	"Tab",
	"PageUp",
	"PageDown",
	"End",
	"Home",
	"Insert",
	"Delete",
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
	"Left",
	"Right",
	"Up",
	"Down",
	"Numpad0",
	"Numpad1",
	"Numpad2",
	"Numpad3",
	"Numpad4",
	"Numpad5",
	"Numpad6",
	"Numpad7",
	"Numpad8",
	"Numpad9",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
};

// Used to map SFML mouse button enums to strings for display in the property window
static const char* kSFMLMouseNames[] = {
	"LeftClick",
	"RightClick",
	"MiddleClick",
	"XButton1",
	"XButton2",
};

#endif // PROPERTYWINDOW_H
