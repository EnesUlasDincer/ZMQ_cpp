#include <zmq.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread> // For simulating delay

// Struct for a 3D point with RGB
struct Point3D {
    float x, y, z;
    int r, g, b; // RGB color
};

// Simulate capturing a point cloud frame from a camera
std::vector<Point3D> capturePointCloudFrame() {
    return {
        {1.0f, 2.0f, 3.0f, 255, 0, 0},
        {4.0f, 5.0f, 6.0f, 0, 255, 0},
        {7.0f, 8.0f, 9.0f, 0, 0, 255}
    };
}

// Serialize a vector of Point3D with RGB to a string
std::string serializePointCloud(const std::vector<Point3D>& points) {
    std::ostringstream oss;
    for (const auto& point : points) {
        oss << point.x << "," << point.y << "," << point.z << ","
            << point.r << "," << point.g << "," << point.b << "\n";
    }
    return oss.str();
}

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://127.0.0.1:2555");

    std::cout << "Connected to server at tcp://127.0.0.1:5555\n";

    while (true) {
        // Simulate capturing a frame from the camera
        std::vector<Point3D> pointCloud = capturePointCloudFrame();

        // Serialize the point cloud
        std::string serializedData = serializePointCloud(pointCloud);

        // Send the serialized point cloud to the server
        zmq::message_t request(serializedData.size());
        memcpy(request.data(), serializedData.c_str(), serializedData.size());
        socket.send(request, zmq::send_flags::none);

        std::cout << "Point cloud sent to server.\n";

        // Wait for acknowledgment
        zmq::message_t reply;
        auto result = socket.recv(reply, zmq::recv_flags::none);
        if (result) {
            std::string replyMsg = reply.to_string();
            std::cout << "Server acknowledgment: " << replyMsg << std::endl;
        }

        // Simulate a delay for the next frame (e.g., 30 FPS -> ~33ms per frame)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    return 0;
}
