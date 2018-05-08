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
    /// Logika interakcji dla klasy BiletPnael.xaml
    /// </summary>
    public partial class BiletPanel : UserControl
    {
        TcpClient client;
        public BiletPanel(TcpClient c)
        {
            InitializeComponent();
            client = c;
        }

        private void Zarezerwuj_Click(object sender, RoutedEventArgs e)
        {
            DaneUzytkownika nwDane = new DaneUzytkownika(client, skad.Content.ToString(), dokad.Content.ToString(), wylot.Content.ToString(), dlugoscLotu.Content.ToString(), miejsce.Content.ToString(), cena.Content.ToString());
            nwDane.Show();
        }

    }
}
