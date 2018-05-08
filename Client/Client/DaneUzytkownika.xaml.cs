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
        string[] dane = new string[6];
        TcpClient client;
        public DaneUzytkownika(TcpClient c, string wylot, string przylot, string data, string dlgLotu, string miejsce, string cena)
        {
            client = c;
            InitializeComponent();
            dane[0] = wylot;
            dane[1] = przylot;
            dane[2] = data;
            dane[3] = dlgLotu;
            dane[4] = miejsce;
            dane[5] = cena;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            File.WriteAllText("dane.txt","Imie: " + imie.Text + '\n' + "Nazwisko: " + nazwisko.Text + '\n' + "Adres email: " + email.Text + '\n' + "Telefon: " + telefon.Text + '\n' + "Miejsce wylotu: " + dane[0] + '\n' + "Miejsce przylotu: " + dane[1] + '\n' + dane[2] + '\n' + dane[3] + '\n' + dane[4] + '\n' + dane[5]);
            var f = File.Open("dane.txt", FileMode.Open);
            NetworkStream stream = client.GetStream();
            var buffer = new byte[1024];
            int bytesRead = 0;
            while ((bytesRead = f.Read(buffer, 0, buffer.Length))> 0)
            {
                stream.Write(buffer, 0, buffer.Length);
            }
            f.Close();
            Close();
        }
    }
}
