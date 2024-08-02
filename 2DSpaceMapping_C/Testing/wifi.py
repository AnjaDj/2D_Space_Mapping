import socket

# IP address and port of the ESP8266
SERVER_IP = '10.99.183.82'  # Change this to your ESP8266's IP
SERVER_PORT = 1492  # Change this to the port your ESP8266 is listening on

def main():
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    #try:
        # Connect to the server
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print("Connected to ESP8266 server.")
    
    while True:
        # Send data to the server
        message = input("Enter message to send: ")
        client_socket.sendall(message.encode())
        
        # Receive response from the server
        data = client_socket.recv(1024)
        print("Received:", data.decode())
            
    #except KeyboardInterrupt:
    #    print("Keyboard interrupt detected. Closing connection.")
    #finally:
        # Close the connection
    #    client_socket.close()
    #    print("Connection closed.")

if __name__ == "__main__":
    main()
