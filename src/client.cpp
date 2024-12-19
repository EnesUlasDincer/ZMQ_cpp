#include <zmq.hpp>
#include <iostream>
#include <string>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    std::cout << "Attempting to connect to the server..." << std::endl;

    socket.connect("tcp://127.0.0.1:2555");
    std::cout << "Connected to server at tcp://127.0.0.1:7730" << std::endl;

    // Send a message to the server
    std::string message = "CreatePointCloud:500";
    zmq::message_t request(message.size());
    memcpy(request.data(), message.c_str(), message.size());

    std::cout << "Sending message..." << std::endl;

    socket.send(request, zmq::send_flags::none);

    std::cout << "Message sent!" << std::endl;

    // Check if the reply was successfully received
    zmq::message_t reply;
    auto result = socket.recv(reply, zmq::recv_flags::none);
    if (!result) {
        std::cerr << "Failed to receive reply\n";
        return 1;
    }

    std::string reply_msg = reply.to_string();
    std::cout << "Received: " << reply_msg << std::endl;

    return 0;
}
