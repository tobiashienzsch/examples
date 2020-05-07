#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;

int server() {
  std::cout << "server" << '\n';
  try {
    // Erase previous message queue
    message_queue::remove("message_queue");

    // Create a message_queue.
    message_queue mq(create_only, "message_queue", 100, sizeof(int));

    // Send 100 numbers
    for (int i = 0; i < 100; ++i) {
      mq.send(&i, sizeof(i), 0);
    }
  } catch (interprocess_exception &ex) {
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}

int client() {
  std::cout << "client" << '\n';

  try {
    // Open a message queue.
    message_queue mq(open_only, "message_queue");

    unsigned int priority;
    message_queue::size_type recvd_size;

    // Receive 100 numbers
    for (int i = 0; i < 100; ++i) {
      int number;
      mq.receive(&number, sizeof(number), recvd_size, priority);
      if (number != i || recvd_size != sizeof(number))
        return EXIT_FAILURE;
      std::cout << std::to_string(number) << '\n';
    }
  } catch (interprocess_exception &ex) {
    message_queue::remove("message_queue");
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
  message_queue::remove("message_queue");
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  if (argc == 2) {
    if (std::string(argv[1]) == "client") {
      return client();
    }
    if (std::string(argv[1]) == "server") {
      return server();
    }
  }

  return EXIT_SUCCESS;
}