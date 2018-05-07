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
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Logika interakcji dla klasy DaneUzytkownika.xaml
    /// </summary>
    public partial class DaneUzytkownika : Window
    {
        TcpClient client;
        public DaneUzytkownika(TcpClient c)
        {
            client = c;
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            File.WriteAllText("dane.txt",imie.Text + '\n' + nazwisko.Text + '\n' + email.Text + '\n' + telefon.Text);
            var f = File.Open("dane.txt", FileMode.Open);
            NetworkStream stream = client.GetStream();
            var buffer = new byte[1024];
            int bytesRead = 0;
            while ((bytesRead = f.Read(buffer, 0, buffer.Length))> 0)
            {
                stream.Write(buffer, 0, buffer.Length);
            }
            f.Close();
        }
    }
}
