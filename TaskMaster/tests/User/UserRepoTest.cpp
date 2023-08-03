#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "UserRepoInterface.hpp"
#include "UserRepo.hpp"
#include "User.hpp"
#include "Task.hpp"
#include "Project.hpp"
#include "Board.hpp"
#include "DbDriverMock.hpp"

using std::vector;
using std::string;

namespace{
    vector<vector<string> > users_ = vector<vector<string> >{
        vector<string>{"1", "Ivan", "Ivanov", "CEO"},
        vector<string>{"2", "Petr", "Petrow", "Team-lead"},
    };
    vector<vector<string> > user_ = vector<vector<string> >{
        vector<string>{"1", "Ivan", "Ivanov", "CEO"}
    };
}

class UserRepoTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(UserRepoTest, GetUsersNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mocAddr, Exec("SELECT * FROM users;"))
        .WillOnce(testing::Return(users_));
    UserRepo repo(mock);
    repo.GetUsers();
}

TEST_F(UserRepoTest, GetUsersDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    UserRepo rep(mock);
    EXPECT_THROW(rep.GetUsers(), std::exception);
}

TEST_F(UserRepoTest, EditUserNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mocAddr, Exec("UPDATE 'users' SET 'name' = 'Ivan' WHERE 'id' = '1' RETURNIING *;"))
        .WillOnce(testing::Return(user_));
    UserRepo rep(mock);
    User user("Ivan");
    user.Id = 1;
    rep.EditUser(user);
}

TEST_F(UserRepoTest, EditUserDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    UserRepo rep(mock);
    User user("Ivan");
    EXPECT_THROW(rep.EditUser(user), std::exception);
}

TEST_F(UserRepoTest, EditUserNoUser){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mocAddr, Exec("UPDATE 'users' SET 'name' = 'Ivan' WHERE 'id' = '1' RETURNING *;"))
        .WillOnce(testing::Return(vector<vector<string> >()));
    UserRepo rep(mock);
    User user("Ivan");
    user.Id = 1;
    EXPECT_THROW(rep.EditUser(user), std::exception);
}


TEST_F(UserRepoTest, AddTaskToUserNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mocAddr, Exec("INSERT INTO 'user_task' (user_id, task_id) VALUES ('1', '1') RETURNING *;"))
        .WillOnce(testing::Return(vector<vector<string> >{vector<string>{ "1", "1"}}));
    User user("Ivan");
    user.Id = 1;
    Task task(1, "To do", "To do");
    task.Id = 1;
    UserRepo rep(mock);
    rep.AddTaskToUser(user, task);
}

TEST_F(UserRepoTest, AddTaskToUserDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    User user("Ivan");
    user.Id = 1;
    Task task(1, "To do", "To do");
    task.Id = 1;
    UserRepo rep(mock);
    EXPECT_THROW(rep.AddTaskToUser(user, task), std::exception);
}

TEST_F(UserRepoTest, AddBoardToUserNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*mocAddr, Exec("INSERT INTO 'user_board' (user_id, board_id) VALUES ('1', '1') RETURNING *;"))
        .WillOnce(testing::Return(vector<vector<string> >{vector<string>{ "1", "1"}}));
    User user("Ivan");
    user.Id = 1;
    Board board(1, 1, "Frontend", "Frontend");
    board.Id = 1;
    UserRepo rep(mock);
    rep.AddBoardToUser(user, board);
}

TEST_F(UserRepoTest, AddBoardToUserDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    User user("Ivan");
    user.Id = 1;
    Board board(1, 1, "Frontend", "Frontend");
    board.Id = 1;
    UserRepo rep(mock);
    EXPECT_THROW(rep.AddBoardToUser(user, board), std::exception);
}

TEST_F(UserRepoTest, GetUsersForProjectDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    UserRepo rep(mock);
    Project proj(1, "TaskMaster");
    proj.Id = 1;
    EXPECT_THROW(rep.GetUsersForProject(proj), std::exception);
}

TEST_F(UserRepoTest, GetUsersForTaskDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    UserRepo rep(mock);
    Task task(1, "To do", "To do");
    task.Id = 1;
    EXPECT_THROW(rep.GetUsersForTask(task), std::exception);
}

TEST_F(UserRepoTest, GetUsersForBoardDisconnect){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(false));
    UserRepo rep(mock);
    Board board(1, 1, "Frontend", "Frontend");
    board.Id = 1;
    EXPECT_THROW(rep.GetUsersForBoard(board), std::exception);
}

TEST_F(UserRepoTest, GetUsersForProjectNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    UserRepo rep(mock);
    Project proj(1, "TaskMaster");
    proj.Id = 1;
    rep.GetUsersForProject(proj);
}

TEST_F(UserRepoTest, GetUsersForTaskNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    UserRepo rep(mock);
    Task task(1, "To do", "To do");
    task.Id = 1;
    rep.GetUsersForTask(task);
}

TEST_F(UserRepoTest, GetUsersForBoardNormal){
    auto mock = std::make_shared<DbDriverMock>();
    auto mocAddr = mock.get();
    EXPECT_CALL(*mocAddr, Connected())
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Return(true));
    UserRepo rep(mock);
    Board board(1, 1, "Frontend", "Frontend");
    board.Id = 1;
    rep.GetUsersForBoard(board);
}













