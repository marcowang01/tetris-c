#include "model.hxx"
#include <catch.hxx>

struct Test_access
{
    Model & model ;
    Board & board ()
    {
        return model.board_ ;
    }

    bool & game_over()
    {
        return model.game_over_;
    }

    Position_set & pset ()
    {
        return model.pset_ ;
    }

};

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate at least six of the functional
// requirements.
//

TEST_CASE ("Line clears (single, multiple, alternating)")
{
    Model m = Model();
    double dt = 2; //run game at 2 FPS

    Test_access t{m};
    t.model.I_mode_ = true;
    t.model.restart_game();
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.ball_falls();
    CHECK_FALSE(t.model.ball_.live);

    t.model.on_frame(dt);
    t.model.on_frame(dt);


    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);


    t.model.rotate();
    t.model.ball_.center.x = 4;
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.rotate();
    t.model.ball_.center.x = 5;
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);
    // std::cout<<t.board();
    CHECK(t.model.score_ == 1); // 1 line cleared
    CHECK(t.pset().size() == 6);

    for (int i = 0; i < 3; i ++){
        Test_access t{m};
        t.model.I_mode_ = true;
        t.model.translate("Left");
        t.model.on_frame(dt);
        t.model.translate("Left");
        t.model.on_frame(dt);
        t.model.translate("Left");
        t.model.on_frame(dt);
        t.model.ball_falls();
        CHECK_FALSE(t.model.ball_.live);

        t.model.on_frame(dt);
        t.model.on_frame(dt);

        t.model.translate("Right");
        t.model.on_frame(dt);
        t.model.translate("Right");
        t.model.on_frame(dt);
        t.model.translate("Right");
        t.model.on_frame(dt);
        t.model.ball_falls();
        t.model.on_frame(dt);
        t.model.on_frame(dt);
    }

    // std::cout<<t.board();
    CHECK(t.model.score_ == 4); // 4 lines cleared total
    CHECK(t.pset().size() == 0);

    t.model.on_frame(dt);
    t.model.ball_falls();

    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.ball_falls();
    CHECK_FALSE(t.model.ball_.live);

    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.rotate();
    t.model.ball_.center.x = 4;
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.rotate();
    t.model.ball_.center.x = 5;
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);
    // std::cout<<t.board();
    CHECK(t.model.score_ == 5); // line clear not containing the bottom row
    CHECK(t.pset().size() == 14);
    Position_set expected_pset = {{3,0},{4,0},{5,0},
                                  {6,0},{7,0},{8,0},
                                  {9,0}, {2,0},
                                  {5,1},{5,2},{5,3},
                                  {4,1},{4,2},{4,3}};
    CHECK(t.pset() == expected_pset);

    t.game_over() = true;
    dt = 1;
    t.model.I_mode_ = true;
    t.model.T_mode_ = false;
    m.restart_game();
    Position_set setup = {{0, 0}, {0, 2}, {1, 0},
                          {1, 1}, {1, 2}, {1, 3},
                          {2, 0}, {2, 2}, {3, 0},
                          {3, 2}, {3, 4}, {4, 0},
                          {4, 1}, {4, 2}, {4, 3},
                          {4, 4}, {4, 5}, {5, 0},
                          {5, 2}, {5, 4}, {6, 0},
                          {6, 2}, {7, 0}, {7, 1},
                          {7, 2}, {7, 3}, {8, 0},
                          {8, 2}};
    for (auto pos : setup){
        t.pset()[pos] = true;
        t.board()[pos] = Player::T;
    }
    m.on_frame(1);
    m.rotate();
    m.on_frame(1);
    m.translate("Right");
    m.on_frame(1);
    m.translate("Right");
    m.on_frame(1);
    m.translate("Right");
    m.on_frame(1);
    m.translate("Right");
    m.ball_falls();
    m.on_frame(1);

    setup = {{1, 0}, {1, 1}, {3, 2}, {4, 0},
             {4, 1}, {4, 2}, {4, 3}, {5, 2},
             {7, 0}, {7, 1}, {9, 0}, {9, 1}};
    CHECK(t.pset() == setup);
}



TEST_CASE ("ball_falls & delay")
{
    Model m = Model();
    double dt = 1.01; //run game at 1 FPS
    Test_access t{m};

    t.model.I_mode_ = true;
    t.model.restart_game();
    CHECK(t.model.ball_.center.y == 20);
    for (int i = 0; i < 20; i ++){
        t.model.on_frame(dt);
    }

    CHECK(t.model.ball_.live);

    CHECK(t.model.ball_.center.y == 0);
    CHECK(t.model.ball_.center.x == 5);
    t.model.translate("Left");
    CHECK(t.model.ball_.center.x == 4);
    t.model.on_frame(dt);
    CHECK(t.model.ball_.center.y == 0);
    CHECK_FALSE(t.model.ball_.live); // ball still not placed
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    CHECK(t.model.ball_.center.y > 0); // after delay new ball generated
    CHECK(t.model.ball_.live);
    for (int i =0; i < 18; i ++){
        t.model.on_frame(dt);
    }
    t.model.on_frame(dt);
    CHECK(t.model.ball_.next(dt, 1).hits_block(t.pset()));
    dt = 0.2;
    t.model.translate("Right");
    t.model.on_frame(dt); // 0.3
    CHECK_FALSE(t.model.ball_.live);
    t.model.translate("Right");
    t.model.on_frame(dt);  // 0.6
    CHECK_FALSE(t.model.ball_.live);
    t.model.translate("Right");
    t.model.on_frame(dt);  // 0.9
    CHECK_FALSE(t.model.ball_.live);
    t.model.on_frame(dt); // 1.2
    t.model.on_frame(dt); // 1.5
    CHECK_FALSE(t.model.ball_.live);
    t.model.on_frame(dt); // 1.8
    CHECK_FALSE(t.model.ball_.live); // still not placed after >1s of being dead
    CHECK(t.model.ball_.center.y == 0);
}


TEST_CASE ("game-over cond. + speed check")
{
    Model m = Model();
    double dt = 0.101;
    Test_access t{m};
    t.model.I_mode_ = true;
    t.model.restart_game();
    CHECK(t.model.ball_.center.y == 20);
    for(int i = 0; i < 10; i ++){
        t.model.translate("Left");
        t.model.on_frame(dt);
    }
    CHECK(t.model.ball_.center.y == 19); // speed is 1 row per second
    t.model.score_ = 30;
    for(int i = 0; i < 10; i ++){
        t.model.on_frame(dt);
    }
    CHECK(t.model.ball_.center.y == 17); // speed is 2 row per second
    t.model.score_ = 139;
    dt = 0.0101;
    for (int i = 0; i < 10; i ++){
        t.model.on_frame(dt);
    }
    CHECK(t.model.ball_.center.y == 7); // speed is 10 row per 0.1 second
    dt = 1;
    t.model.ball_falls();
    t.model.on_frame(dt);

    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.rotate();
    t.model.ball_.center.x = 4;
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    t.model.rotate();
    t.model.ball_.center.x = 5;
    CHECK(m.score_ == 139);
    t.model.ball_falls();
    t.model.on_frame(dt);
    t.model.on_frame(dt);
    CHECK(m.score_ == 140);
    t.model.on_frame(dt);
    CHECK(m.is_game_over());
    t.model.on_frame(dt);

    CHECK_FALSE(m.ball_.live);

    t.model.restart_game();
    CHECK(m.score_ == 0);
    CHECK(t.pset().size() == 0);
    CHECK(m.ball_.live);
    dt = 5;

    for (int i = 0 ; i<5; i++){
        t.model.on_frame(dt);
        t.model.rotate();
        t.model.on_frame(dt);
        t.model.ball_falls();
        t.model.on_frame(dt);
    }

    t.model.on_frame(dt);
    t.model.on_frame(dt);
    t.model.on_frame(dt);
    // std::cout<<t.board();
    CHECK(t.model.is_game_over());


}

TEST_CASE ("Wall-kick + translate")
{
    Model m = Model();
    double dt = 1; //run game at 1 FPS

    Test_access t{m};
    t.model.T_mode_ = true;
    t.model.restart_game();

    t.model.rotate();
    t.model.on_frame(dt); //time = 1

    for (int i = 0 ; i < 11; i++){

        t.model.translate("Left");
        t.model.on_frame(dt);
    }

    t.model.rotate();
    t.model.ball_falls();
    t.model.on_frame(dt);

    Position_set exp_pset = {{0,1},{1,1}, {2,1}, {1,0}}; // center piece is

    CHECK( t.pset() == exp_pset);

}


TEST_CASE ("Wall-kick + translate + blocked")
{
    Model m = Model();
    double dt = 1; //run game at 1 FPS

    Test_access t{m};
    t.model.T_mode_ = true;
    t.model.restart_game();


    t.model.on_frame(dt); //time = 1

    for (int i = 0; i < 3; i++) {

        t.model.translate("Left");
        t.model.on_frame(dt);
    }

    t.model.ball_falls();
    t.model.on_frame(dt);

    for (int i = 0; i < 3; i++) {

        t.model.translate("Left");
        t.model.on_frame(dt);
    }
    t.model.ball_falls();
    t.model.on_frame(dt);

    // this piece is to be rotated in an impossible location

    t.model.rotate();
    for (int i = 0; i < 6; i++) {

        t.model.translate("Left");
        t.model.on_frame(dt);
    }

    for (int ii = 0; ii < 17; ii++) {
        t.model.on_frame(dt);
    }
    dt = 0.3;
    t.model.rotate();
    //attempt to rotate in impossible spot --> should do nothing
    t.model.on_frame(dt);
    t.model.translate("Right");
    t.model.on_frame(dt);
    t.model.translate("Left");
    t.model.on_frame(dt);
    t.model.on_frame(dt);

    Position_set exp_pset = {{{0, 2}, {0, 3}, {0, 4},
                              {1, 0}, {1, 2}, {1, 3},
                              {2, 0}, {2, 1}, {2, 2},
                              {2, 3}, {3, 0}, {3, 2}}};

    std::cout << t.board();
    CHECK( t.pset() == exp_pset);

}