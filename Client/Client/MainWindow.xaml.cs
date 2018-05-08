using System;
using System.Collections.Generic;
using System.IO;
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
        public TcpClient client;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Polacz(adresIP.Text, port.Text);
        }
 


        public void Polacz(string SERVER_IP, string PORT_NO)
        {
            client = new TcpClient(SERVER_IP, Convert.ToInt32(PORT_NO));
            NetworkStream nwStream = client.GetStream();
            var output = File.Create("result.txt");
            
            var buffer = new byte[1024];
            int bytesRead = 0;
            var filesize = new byte[8];
            nwStream.Read(filesize, 0, filesize.Length);
            long fs = BitConverter.ToInt64(filesize,0);
            while ((bytesRead = nwStream.Read(buffer, 0, buffer.Length)) > 100)
            {
                output.Write(buffer, 0, bytesRead);
            }

            output.Close();
            Bilety bilety = new Bilety(client);
            bilety.Show();
            Close();
        }

        private void adresIP_TextChanged(object sender, TextChangedEventArgs e)
        {
        }

        private void port_TextChanged(object sender, TextChangedEventArgs e)
        {
        }
    }
}
