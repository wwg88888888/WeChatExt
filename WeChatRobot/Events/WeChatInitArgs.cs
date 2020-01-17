using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WeChat.Events
{
    public class WeChatInitArgs
    {
        /// <summary>
        /// 是否成功
        /// </summary>
        public bool IsSuccess { get; set; }

        public int Error { get; set; }

        public string Message { get; set; }  

        public int PorcessId { get; set; }
    }
}
