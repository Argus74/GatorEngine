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
    void DrawComponent(const char *name, const T &component);

    // Draw the properties of a component
    void DrawComponentProperties(std::shared_ptr<CTransform> transform);
    void DrawComponentProperties(std::shared_ptr<CName> name);
    void DrawComponentProperties(std::shared_ptr<CShape> shape);
    void DrawComponentProperties(std::shared_ptr<CBBox> bbox);

    // Draw one property row of a component
    template <typename T>
    void DrawProperty(const char *name, T &value);

    // Draw an input field (text box, color picker, etc.) for a property row
    // TODO: May need to update parameters, like switching to our new Vec2 object
    void DrawInputField(std::string &val);
    void DrawInputField(sf::Color &val);
    void DrawInputField(sf::Vector2f &val);
    void DrawInputField(float &val);
    void DrawInputField(int &val);
    void DrawInputField(bool &val);

    // ImGui customization options for tables (the rows under each component header)
    ImGuiTableFlags table_flags;

    // ImGui customization options for tree nodes (the component header bars)
    ImGuiTreeNodeFlags tree_node_flags;
};

#endif // PROPERTYWINDOW_H
