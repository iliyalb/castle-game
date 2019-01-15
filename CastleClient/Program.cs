using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

namespace CastleClient
{
    public class Client : MonoBehaviour
    {
        #region fields
        private static Socket _clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static List<SocketException> _clientSocketError = new List<SocketException>();
        #endregion

        public void Connect(string ip)
        {
            StartCoroutine(AttemptConnect(ip));
        }
        public IEnumerator AttemptConnect(string ip)
        {
            if (ip != "local")
            {
                if (Application.internetReachability != NetworkReachability.NotReachable)
                {
                    Debug.Log("Network rechable");

                    int attempts = 0;

                    while (!_clientSocket.Connected)
                    {
                        try
                        {
                            attempts++;
                            _clientSocket.Connect(IPAddress.Parse(ip), 100);
                        }
                        catch (SocketException)
                        {
                            Debug.Log("Connection attempts: " + attempts.ToString());
                            if (attempts == 2)
                                Debug.Log("Connection failed after " + attempts + " attempts");
                        }
                        yield return new WaitForSeconds(3);
                    }
                    Debug.Log("Connected");
                }
                else
                {
                    Debug.Log("Network Unrechable");
                }
            }
            else
            {
                Debug.Log("Loopback");

                int attempts = 0;

                while (!_clientSocket.Connected)
                {
                    try
                    {
                        attempts++;
                        _clientSocket.Connect(IPAddress.Loopback, 100);
                    }
                    catch (SocketException)
                    {

                        Debug.Log("Connection attempts: " + attempts.ToString());
                        if (attempts == 2)
                            Debug.Log("Connection failed after " + attempts + " attempts");
                    }
                    yield return new WaitForSeconds(3);
                }
                Debug.Log("Connected");
            }
        }
        public static void Send(string req)
        {
            byte[] buffer = Encoding.ASCII.GetBytes(req);

            try
            {
                _clientSocket.Send(buffer);
            }
            catch (SocketException se)
            {
                Debug.Log("Connection failed");
                _clientSocketError.Add(se);
            }

            byte[] receivedBuffer = new byte[1024];

            try
            {
                int rec = _clientSocket.Receive(receivedBuffer);
                byte[] data = new byte[rec];
                Array.Copy(receivedBuffer, data, rec);
                Debug.Log("Received: " + Encoding.ASCII.GetString(data));
            }
            catch (SocketException se)
            {
                _clientSocketError.Add(se);
            }
            if (req == "exit")
                Exit();
            if (req == "debug")
            {
                if (_clientSocketError.Count > 0)
                    _clientSocketError.ForEach(i => Debug.Log(i + "\n"));
                else
                    Debug.Log("No socket errors found");
            }
        }
        public static void Exit()
        {
            System.Environment.Exit(0);
        }
    }
}