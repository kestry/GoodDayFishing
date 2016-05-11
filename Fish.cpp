//void Fish::update(World &world) {
//    if (first_x_ == MIN_FIRST_X) {
//        boating_x_velocity_ = -boating_x_velocity_;
//    }
//    first_x_ += boating_x_velocity_;
//    if (first_x_ == MAX_FIRST_X + 5) {
//        boating_x_velocity_ = -boating_x_velocity_;
//    }
//    (boating_x_velocity_ < 0) ?
//        boating_left_sprite_.draw(world, first_x_, first_y_) :
//        boating_right_sprite_.draw(world, first_x_, first_y_);
//
//}