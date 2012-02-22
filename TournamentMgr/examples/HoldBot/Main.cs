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
        public Main(string host, int port)
        {
            InitializeComponent();
            baseHost = host;
            basePort = port;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            List<String> botList = new List<string>(); // A list of the addresses of every bot
            String fileLocation = this.textBox1.Text + "\\aiserver.exe";
            String runCommand = "/C cd ";

            // Checks
            if (!File.Exists(@fileLocation))
            {
                MessageBox.Show("Error: Could not locate AiServer.exe at " + fileLocation);
                return;
            }
            else if (Int32.Parse(this.textBox2.Text) < 1)
            {
                MessageBox.Show("Error: Please provide a positive number of rounds. Number supplied: " + this.textBox2.Text);
                return;
            }
            else if (Int32.Parse(this.textBox3.Text) < 0)
            {
                MessageBox.Show("Error: Please provide a press level between 0 and 8000.");
                return;
            }
            else if (Int32.Parse(this.textBox4.Text) < 0)
            {
                MessageBox.Show("Error: Please provide a positive non-zero time limit.");
                return;
            }

            // Setup the arguments to aiserver.exe
            runCommand += this.textBox1.Text + " && aiserver.exe -start -h -exit=1 -lvl=" + this.textBox3.Text; // Press level
            runCommand += " -kill=" + this.textBox4.Text;
            runCommand += " -mtl=" + this.textBox5.Text + " -rtl=" + this.textBox5.Text + " -btl=" + this.textBox5.Text; // Timeouts

            // Build up the botList
            if (textBox6.Text.Length > 0 && File.Exists(@textBox6.Text)) botList.Add(textBox6.Text);
            if (textBox7.Text.Length > 0 && File.Exists(@textBox7.Text)) botList.Add(textBox7.Text);
            if (textBox8.Text.Length > 0 && File.Exists(@textBox8.Text)) botList.Add(textBox8.Text);
            if (textBox9.Text.Length > 0 && File.Exists(@textBox9.Text)) botList.Add(textBox9.Text);
            if (textBox10.Text.Length > 0 && File.Exists(@textBox10.Text)) botList.Add(textBox10.Text);
            if (textBox11.Text.Length > 0 && File.Exists(@textBox11.Text)) botList.Add(textBox11.Text);
            if (textBox12.Text.Length > 0 && File.Exists(@textBox12.Text)) botList.Add(textBox12.Text);

            if (botList.Count == 0)
            {
                MessageBox.Show("Please provide at least one valid bot.");
                return;
            }

            /*
             * Let the tournament begin!
             */

            // First disable the to run again
            this.button2.Enabled = false;

            // Backup any old tournament files
            if (File.Exists("TournamentResults.csv"))
            {
                // Not going to keep endless backups
                if (File.Exists("results" + DateTime.Now.ToString("ddMMyyyy"))) File.Delete("results" + DateTime.Now.ToString("ddMMyyyy"));
                File.Move("TournamentResults.csv", "results" + DateTime.Now.ToString("ddMMyyyy"));
            }

            for(int roundsCompleted = 0; roundsCompleted < Int32.Parse(this.textBox2.Text); roundsCompleted++)
            {
                // Load up the server and our monitor
                Process Server = new Process();
                Server.StartInfo = new ProcessStartInfo("cmd.exe", runCommand);
                Server.Start();

                // Start up our observer
                Process Watcher = new Process();
                Watcher.StartInfo = new ProcessStartInfo("Watcher.exe");
                Watcher.Start();

                // Give the Watcher time to connect
                System.Threading.Thread.Sleep(3000);

                if (!Server.Responding) MessageBox.Show("Server is not yet responding.");

                // Stick the bots in
                int rndSeed = (int)DateTime.Now.Ticks;
                Random rnd = new Random(rndSeed);
                List<Process> processList = new List<Process>(); // Contain all the processes that the bots sit in

                Process proc = new Process();

                for (int i = 0; i < 7; i++)
                {
                    Process botProcess = new Process();
                    rndSeed += rnd.Next(50);
                    if (radioButton1.Checked && (i < (((int) 7 / botList.Count) * botList.Count) )) // We want even numbers of bots
                    {
                        proc.StartInfo = new ProcessStartInfo(botList[(int) i % botList.Count]);
                        proc.Start();
                        processList.Add(proc);
                    }
                    else
                    {
                        rnd = new Random(rndSeed);
                        proc.StartInfo = new ProcessStartInfo(botList[rnd.Next(botList.Count)]); // Select a bot at random
                        proc.Start();
                        processList.Add(proc);
                    }
                }

                // Pass time until a result comes in
                while (!Server.HasExited)
                {
                    System.Threading.Thread.Sleep(200);
                }

                // Try to clean up a little
                foreach (Process p in processList)
                {
                    // Make sure its dead
                    try
                    {
                        p.Close();
                        p.Kill();
                    }
                    catch
                    {
                    }                    
                }
            }

            MessageBox.Show("Tournament completed successfully.\n");
            this.button2.Enabled = true;
        }

        private void Browse_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog newBox = new FolderBrowserDialog();
            newBox.ShowDialog();
            this.textBox1.Text = newBox.SelectedPath.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox6.Text = newBox.FileName.ToString();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox7.Text = newBox.FileName.ToString();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox8.Text = newBox.FileName.ToString();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox9.Text = newBox.FileName.ToString();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox10.Text = newBox.FileName.ToString();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox11.Text = newBox.FileName.ToString();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            OpenFileDialog newBox = new OpenFileDialog();
            newBox.ShowDialog();
            this.textBox12.Text = newBox.FileName.ToString();
        }
    }
}