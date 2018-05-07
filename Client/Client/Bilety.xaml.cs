using System;
using System.Collections.Generic;
using System.Linq;
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
using System.IO;
using System.Net.Sockets;

namespace Client
{
    /// <summary>
    /// Logika interakcji dla klasy Bilety.xaml
    /// </summary>
    public partial class Bilety : Window
    {
        public TcpClient client;
        public Bilety(TcpClient c)
        {
            client = c;
            InitializeComponent();
            ZaladujBilety();
        }

        void ZaladujBilety()
        {
            StreamReader sr = new StreamReader("result.txt");
            String line = sr.ReadToEnd();
            string[] wyrazy = line.Split('#');
            for (int i = 0; i < wyrazy.Length / 6; i++)
            {
                BiletPanel bp = new BiletPanel(client);
                bp.skad.Content = wyrazy[(i*6)];
                bp.dokad.Content = wyrazy[(i * 6) + 1];
                bp.wylot.Content = "Data: " + wyrazy[(i * 6) + 2];
                bp.dlugoscLotu.Content = "Czas: " + wyrazy[(i * 6) + 3];
                bp.miejsce.Content = "Miejsce: " + wyrazy[(i * 6) + 4];
                bp.cena.Content = "Cena: " + wyrazy[(i * 6) + 5];
                rootGrid.Children.Add(bp);
                bp.Margin = new Thickness(0, 80 * i, 0, 0);
            }
        }
    }
}
