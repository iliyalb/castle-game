using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace CastleServer
{
    [Serializable]
    class Program
    {
        private static readonly Socket _serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static readonly List<Socket> _clientSockets = new List<Socket>();
        private const int BUFFER_SIZE = 8192;
        private const int PORT = 100;
        private static byte[] _buffer = new byte[BUFFER_SIZE];

        //public static bool[] _tutorial;

        static void Main(string[] args)
        {
            Console.Title = "Castle Server";
            Console.WriteLine(@"_________                  __  .__             _________                                ");
            Console.WriteLine(@"\_   ___ \_____    _______/  |_|  |   ____    /   _____/ ______________  __ ___________ ");
            Console.WriteLine(@"/    \  \/\__  \  /  ___/\   __\  | _/ __ \   \_____  \_/ __ \_  __ \  \/ // __ \_  __ \");
            Console.WriteLine(@"\     \____/ __ \_\___ \  |  | |  |_\  ___/   /        \  ___/|  | \/\   /\  ___/|  | \/");
            Console.WriteLine(@" \______  (____  /____  > |__| |____/\___  > /_______  /\___  >__|    \_/  \___  >__|   ");
            Console.WriteLine(@"        \/     \/     \/                 \/          \/     \/                 \/       ");
            Console.WriteLine();
            SetupServer();
            Console.ReadLine();
            CloseAllSockets();

        }

        private static void SetupServer()
        {
            Console.WriteLine(DateTime.Now + " Setting up server...");
            _serverSocket.Bind(new IPEndPoint(IPAddress.Any, PORT));
            _serverSocket.Listen(1000);
            _serverSocket.BeginAccept(AcceptCallback, null);
            Console.WriteLine(DateTime.Now + " Server up and running.");
        }

        private static void CloseAllSockets()
        {
            foreach (Socket socket in _clientSockets)
            {
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }
            _serverSocket.Close();
        }

        private static void AcceptCallback(IAsyncResult AR)
        {
            Socket socket;
            try
            {
                socket = _serverSocket.EndAccept(AR);
            }
            catch (ObjectDisposedException)
            {
                return;
            }
            _clientSockets.Add(socket);
            socket.BeginReceive(_buffer, 0, BUFFER_SIZE, SocketFlags.None, ReceiveCallback, socket);
            Console.WriteLine(DateTime.Now + " Remote: " + socket.RemoteEndPoint.ToString() + " connected. Buffer size: "
                + socket.ReceiveBufferSize.ToString() + " Timeout: " + socket.ReceiveTimeout.ToString());
            _serverSocket.BeginAccept(AcceptCallback, null);
        }

        private static void ReceiveCallback(IAsyncResult AR)
        {
            Socket current = (Socket)AR.AsyncState;
            int received;
            try
            {
                received = current.EndReceive(AR);
            }
            catch (SocketException)
            {
                Console.WriteLine(DateTime.Now + " " + current.RemoteEndPoint.ToString() + " Client disconnected");
                current.Close();
                _clientSockets.Remove(current);
                return;
            }
            byte[] dataBuffer = new byte[received];
            Array.Copy(_buffer, dataBuffer, received);
            string text = Encoding.ASCII.GetString(dataBuffer);
            Console.WriteLine(DateTime.Now + " " + current.RemoteEndPoint.ToString() + " received: " + text);

            string response = string.Empty;

            text = text.ToLower();

            switch (text)
            {
                case "debug":
                    {
                        byte[] data = Encoding.ASCII.GetBytes("DEBUG CODE RECEIVED");
                        current.Send(data);
                        break;
                    }
                case "get time":
                    {
                        Console.WriteLine(DateTime.Now + " REQUEST: get time");
                        byte[] data = Encoding.ASCII.GetBytes(DateTime.Now.ToLongTimeString());
                        current.Send(data);
                        Console.WriteLine(DateTime.Now + " REPLY: get time");
                        break;
                    }
                case "exit":
                    {
                        current.Shutdown(SocketShutdown.Both);
                        current.Close();
                        _clientSockets.Remove(current);
                        Console.WriteLine(DateTime.Now + " Client disconnected");
                        return;
                    }
                default:
                    {
                        Console.WriteLine(DateTime.Now + " Invalid input: " + text);
                        byte[] data = Encoding.ASCII.GetBytes("Inavlid request");
                        current.Send(data);
                        Console.WriteLine(DateTime.Now + " " + current.RemoteEndPoint.ToString() + " Warning sent");
                        break;
                    }
            }
            current.BeginReceive(_buffer, 0, BUFFER_SIZE, SocketFlags.None, ReceiveCallback, current);
        }
    }
}