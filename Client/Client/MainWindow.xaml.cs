using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Dupa(adresIP.Text, port.Text);
        }

        public void Dupa(string SERVER_IP, string PORT_NO)
        {
            Status.Foreground = new SolidColorBrush(Colors.Green);
            Status.Content = "Online";

            string textToSend = DateTime.Now.ToString();

            TcpClient client = new TcpClient(SERVER_IP, Convert.ToInt32(PORT_NO));
            NetworkStream nwStream = client.GetStream();
            byte[] bytesToSend = ASCIIEncoding.ASCII.GetBytes(textToSend);

            Console.WriteLine("Sending : " + textToSend);
            nwStream.Write(bytesToSend, 0, bytesToSend.Length);

            byte[] bytesToRead = new byte[client.ReceiveBufferSize];
            int bytesRead = nwStream.Read(bytesToRead, 0, client.ReceiveBufferSize);
            Console.WriteLine("Recieved: " + Encoding.ASCII.GetString(bytesToRead, 0, bytesRead));
            Wiadomosc.Content = Encoding.ASCII.GetString(bytesToRead, 0, bytesRead);
            Console.ReadLine();
            client.Close();
        }
    }
}
