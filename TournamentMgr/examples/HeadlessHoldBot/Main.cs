using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;
using Polarsoft.Diplomacy.Daide;

namespace Polarsoft.Diplomacy.AI
{
    public partial class Main : Form
    {
        string baseHost;
        int basePort;
        BaseBot bot;

        public Main(string host, int port)
        {
            InitializeComponent();
            baseHost = host;
            basePort = port;
            this.bot = new Watcher(host, port, this);
        }
    }
}