using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WeChat
{
    public class SocketRespons
    {
        public int Code { get; set; }

        public int Error { get; set; }

        public string Message { get; set; }

        public int Pid { get; set; }

        public string Callback { get; set; }

        public JToken Data { get; set; } 
    }
}
