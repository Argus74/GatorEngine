#include "CTransform.h"

CTransform::CTransform(Vec2 position, Vec2 velocity) {
    this->position = position;
    this->velocity = velocity;
}

void CTransform::Translate(sf::Vector2f offset) {
    transform_.translate(offset);
}

void CTransform::SetPosition(sf::Vector2f position) {
    const float* old_transform_data = transform_.getMatrix();

    transform_ = sf::Transform(old_transform_data[0], old_transform_data[1], old_transform_data[2],
                               old_transform_data[3], old_transform_data[4], old_transform_data[5],
                               position.x, position.y, 1);
}

sf::Vector2f CTransform::GetPosition() {
    const float* values = transform_.getMatrix();
    return sf::Vector2f(values[12], values[13]);
}
