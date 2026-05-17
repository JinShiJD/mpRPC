#include "test.pb.h"
#include <iostream>
#include <string>

using namespace fixbug;

int main()
{
    // LoginResponse rsp;
    // // 一个类型中的一个变量的类型 来自 另一个类型时 的使用
    // ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(0);

    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();
    user1->set_name("star");
    user1->set_age(22);
    user1->set_sex(User::WOMAN);
    //user1->set_sex(User_Sex::User_Sex_WOMAN);

    User *user2 = rsp.add_friend_list();
    user1->set_name("link");
    user1->set_age(23);
    user1->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;



    return 0;
}



int main1()
{
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("Star");
    req.set_pwd("777888");

    // 对象数据序列化 ==>> char*
    std::string send_str;
    if(req.SerializeToString(&send_str))
    {
        std::cout << send_str.c_str() << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str))  
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}

/*
protoc test.proto --cpp_out=./
g++ main.cc test.pb.cc -lprotobuf
*/