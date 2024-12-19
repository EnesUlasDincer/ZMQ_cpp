#include <zmq.hpp>
#include <iostream>
#include <string>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:2555");

    std::cout << "Server is running on tcp://127.0.0.1:5555\n";

    while (true) {
        zmq::message_t request;

        std::cout << "Waiting for a message..." << std::endl;
        auto result = socket.recv(request, zmq::recv_flags::none);
        if (!result) {
            std::cerr << "Failed to receive message" << std::endl;
            continue;
        }

        std::cout << "Message received!" << std::endl;
        std::string received_msg = request.to_string();
        std::cout << "Received: " << received_msg << std::endl;

        // Send a reply
        std::string response = "Server Response: " + received_msg;
        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.data(), response.size());
        socket.send(reply, zmq::send_flags::none);



    }

    return 0;
}
