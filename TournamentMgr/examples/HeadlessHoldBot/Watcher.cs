using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using Polarsoft.Diplomacy;
using Polarsoft.Diplomacy.Orders;
using Polarsoft.Diplomacy.Daide;

namespace Polarsoft.Diplomacy.AI
{
    class Watcher : BaseBot
    {
        Form parent;
        Dictionary<String, Pair<int, int>> botResults = new Dictionary<String, Pair<int, int>>();
        int gamesRun = 0;
        int gamesDrawn = 0;

        public Watcher(string host, int port, Form parent)
            : base()
        {
            this.parent = parent;
            base.Connect(host, port, "Tournament Watcher", "0.1");
        }

        protected override void ProcessNOW(TokenMessage msg)
        {
        }

        protected override void ProcessSMR(TokenMessage msg)
        {
            bool Solo = false;
            // Read in any tournament information already present
            readResultsFile();

            for (int i = 2; i < msg.SubMessages.Count; i++)
            {
                bool Winner = false;
                TokenMessage tm = msg.SubMessages[i];

                if (Int32.Parse(tm.SubMessages[3].ToString()) > 17)
                {
                    Winner = true;
                    Solo = true;
                }
                botCompeted(tm.SubMessages[1].ToString(), Winner);
            }

            gamesRun++;
            if (!Solo) gamesDrawn++;

            writeResultsFile();
        }

        delegate void VoidDelegate();

        // Read in the previous rounds results, if there are any
        private void readResultsFile()
        {
            String currentLine;
            if(!File.Exists("TournamentResults.csv")) 
                return;

            TextReader Tr = File.OpenText("TournamentResults.csv");

            Tr.ReadLine();
            currentLine = Tr.ReadLine();
            gamesRun = Int16.Parse(currentLine.Split(",".ToCharArray())[0]);
            gamesDrawn = Int16.Parse(currentLine.Split(",".ToCharArray())[1]);

            // Dead space
            Tr.ReadLine();
            Tr.ReadLine();

            // Read in the bots
            botResults.Clear();
            while ((currentLine = Tr.ReadLine()) != null)
            {
                Pair<int, int> botData = new Pair<int,int>();
                botData.First = Int16.Parse(currentLine.Split( ",".ToCharArray() )[1]);
                botData.Second = Int16.Parse(currentLine.Split(",".ToCharArray())[2]);
                botResults.Add(currentLine.Split(",".ToCharArray())[0], botData);
            }

            // Finished!
            Tr.Close();
            return;
        }

        // Write the latest results
        private void writeResultsFile()
        {
            // Update information


            // Write the output
            using (TextWriter Tw = File.CreateText("TournamentResults.csv"))
            {
                Tw.WriteLine("Games Played,Games Drawn");
                Tw.WriteLine(gamesRun.ToString() + "," + gamesDrawn.ToString());
                Tw.WriteLine("");
                Tw.WriteLine("Bot,Plays,Wins");

                foreach (String bot in botResults.Keys)
                    Tw.WriteLine(bot + "," + botResults[bot].First + "," + botResults[bot].Second);
            }
        }

        private void botCompeted(String botName, bool Solo)
        {
            // The tournament clearly just started
            if (!botResults.ContainsKey(botName))
            {
                Pair<int, int> botData = new Pair<int, int>();
                botData.First = 1;
                if (Solo) botData.Second = 1;
                else botData.Second = 0;
                botResults.Add(botName, botData);
            }

            // The tournament is already going
            botResults[botName].First++; // Another game played

            if (Solo) botResults[botName].Second++;
        }

        protected override void ProcessOFF(TokenMessage msg)
        {
            base.ProcessOFF(msg);

            // Key to start a new game
            this.parent.Invoke(new VoidDelegate(delegate
            {
                this.parent.Close();
            }));
        }
    }

    public class Pair<T, U>
    {
        public Pair()
        {
        }

        public Pair(T first, U second)
        {
            this.First = first;
            this.Second = second;
        }

        public T First { get; set; }
        public U Second { get; set; }
    };
}

