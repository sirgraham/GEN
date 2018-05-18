//------------------------------------------------------------------------------
// <copyright file="FileHeaderGEN.cs" company="GEN Group">
//  Copyright (c) GEN Group.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.ComponentModel.Design;
using Microsoft.VisualStudio.Shell;
using EnvDTE;
using System.IO;

namespace Extensions
{
    /// <summary>
    /// Command handler
    /// </summary>
    internal sealed class FileHeaderGEN
    {
        string author = "";
        string enterprise = "";

        /// <summary>
        /// Command ID.
        /// </summary>
        public const int CommandId = 0x0102;

        /// <summary>
        /// Command menu group (command set GUID).
        /// </summary>
        public static readonly Guid CommandSet = new Guid("f1afa488-901c-4e32-968e-37155ebb3d2e");

        /// <summary>
        /// VS Package that provides this command, not null.
        /// </summary>
        private readonly Package package;

        /// <summary>
        /// Initializes a new instance of the <see cref="FileHeader"/> class.
        /// Adds our command handlers for menu (commands must exist in the command table file)
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        private FileHeaderGEN(Package package)
        {
            if (package == null)
            {
                throw new ArgumentNullException("package");
            }

            this.package = package;

            OleMenuCommandService commandService = this.ServiceProvider.GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (commandService != null)
            {
                var menuCommandID = new CommandID(CommandSet, CommandId);
                var menuItem = new MenuCommand(this.MenuItemCallback, menuCommandID);
                commandService.AddCommand(menuItem);
                try
                {
                    DTE dte = (DTE)ServiceProvider.GetService(typeof(DTE));
                    string userfoldername = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);

                    using (StreamReader sr = new StreamReader(userfoldername + "\\MacrosAuthor.txt"))
                    {
                        String line = sr.ReadToEnd();
                        string[] array = line.Split('\n');
                        author = array[0].Replace('\r', ' ');
                        enterprise = array[1].Replace('\r', ' ');
                    }
                }
                catch (Exception)
                {
                    author = "No se ha podido abrir el fichero de author";
                }
            }
        }

        /// <summary>
        /// Gets the instance of the command.
        /// </summary>
        public static FileHeaderGEN Instance
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the service provider from the owner package.
        /// </summary>
        private IServiceProvider ServiceProvider
        {
            get
            {
                return this.package;
            }
        }

        /// <summary>
        /// Initializes the singleton instance of the command.
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        public static void Initialize(Package package)
        {
            Instance = new FileHeaderGEN(package);
        }

        /// <summary>
        /// This function is the callback used to execute the command when the menu item is clicked.
        /// See the constructor to see how the menu item is associated with this function using
        /// OleMenuCommandService service and MenuCommand class.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event args.</param>
        private void MenuItemCallback(object sender, EventArgs e)
        {
            DTE dte = (DTE)ServiceProvider.GetService(typeof(DTE));

            if (dte.ActiveDocument != null)
            {
                dte.ActiveDocument.Save();
                var  dtename = dte.ActiveDocument.FullName;
                var  ext     = Path.GetExtension(dtename);
                var  name    = Path.GetFileName(dtename);
                var  group   = "";
                bool isGEN   = false;

                string directory = "";

                directory = Path.GetDirectoryName(dtename);

                GetGroupID(directory, ref isGEN, ref group);

                if (ext == ".cpp")
                  {
                    ProcessCPPHeader(dte, isGEN, name, group);
                  }

                if(ext == ".hpp" || ext == ".h")
                  {
                    int size = ext.Length;
                    ext = ext.Substring(1, (size - 1));

                    ProcessHHeader(dte, isGEN, name, ext, group);
                  }                
            }
        }

        private void GetGroupID(string directory, ref bool isGEN, ref string groupID)
        {
            // is GEN?
            if (directory.Contains("GENFrameWork"))             isGEN = true;

            // Types
            if (directory.Contains("Utils"))                    groupID = "UTILS";
            if (directory.Contains("DataIO"))                   groupID = "DATAIO";
            if (directory.Contains("Graphic"))                  groupID = "GRAPHIC";
            if (directory.Contains("Sound"))                    groupID = "SOUND";
            if (directory.Contains("Input"))                    groupID = "INPUT";
            if (directory.Contains("Cipher"))                   groupID = "CIPHER";
            if (directory.Contains("Compress"))                 groupID = "COMPRESS";
            if (directory.Contains("Main"))                     groupID = "MAIN";
            if (directory.Contains("Script"))                   groupID = "SCRIPT";
            if (directory.Contains("DatabasesSQL"))             groupID = "DATABASESSQL";
            if (directory.Contains("UserInterface"))            groupID = "USERINTERFACE";
            if (directory.Contains("Video"))                    groupID = "VIDEO";
            if (directory.Contains("Media"))                    groupID = "MEDIA";
            if (directory.Contains("Physics"))                  groupID = "PHYSICS";
            if (directory.Contains("ArtificialIntelligence"))   groupID = "ARTIFICIALINTELLIGENCE";

            // Platforms
            if (directory.Contains("Windows"))                  groupID = "PLATFORM_WINDOWS";
            if (directory.Contains("Linux"))                    groupID = "PLATFORM_LINUX";
            if (directory.Contains("Android"))                  groupID = "PLATFORM_ANDROID";
            if (directory.Contains("Common"))                   groupID = "PLATFORM_COMMON";
            if (directory.Contains("STM32Fxxx"))                groupID = "PLATFORM_STM32FXXX";
        }

        private void CreateInfo(bool isGEN, string name, string description, string group, ref string info)
        {
            string date           = DateTime.Now.ToString();
            var    namewithoutext = Path.GetFileNameWithoutExtension(name);

            info  = "*\n";
            info += "*\t@file        " + name + "\n";
            info += "*\n";
            info += "*\t@class       " + (namewithoutext.ToUpper()) + "\n";
            info += "*\t@brief       " + description + "\n";
            info += "*\t@ingroup     " + group + "\n";
            info += "*\n";
            info += "*\t@author      " + author + "\n";
            info += "*\t@date        " + date + "\n";        
            info += "*\n";
        }

        private void CreateCopyright(bool isGEN,  ref string copyright)
        {           
            if(isGEN)
              {         
                copyright  = "*\t@copyright   Copyright(c) 2005 - " + DateTime.Now.Year + " GEN Group.\n";
                copyright += "*\n";
                copyright += "*\t@cond\n";    
                copyright += "*\tPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated\n";
                copyright += "*\tdocumentation files(the \"Software\"), to deal in the Software without restriction, including without limitation\n";
                copyright += "*\tthe rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,\n";
                copyright += "*\tand to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n";
                copyright += "*\n";
                copyright += "*\tThe above copyright notice and this permission notice shall be included in all copies or substantial portions of\n";
                copyright += "*\tthe Software.\n";
                copyright += "*\n";
                copyright += "*\tTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO\n";
                copyright += "*\tTHE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n";
                copyright += "*\tAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n";
                copyright += "*\tTORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n";
                copyright += "*\tSOFTWARE.\n";                
                copyright += "*\t@endcond\n";   
                copyright += "*\n";                
             }
            else
             {
                copyright = "*\t@copyright   Copyright(c) " + DateTime.Now.Year + " " + enterprise + " All rights reserved.\n";
                copyright += "*\n";   
             }           
        }

        private void ProcessCPPHeader(DTE dte, bool isGEN, string name, string group)
        {
            var    selection        = (TextSelection)dte.ActiveDocument.Selection;
            string text             = selection.Text;           
            var    namewithoutext   = Path.GetFileNameWithoutExtension(name);
            string info             = "";
            string copyright        = "";
            string description      = Microsoft.VisualBasic.Interaction.InputBox(   "Purpose of the file"   ,
                                                                                    "File module C, CPP"    ,
                                                                                    "");

            selection.GotoLine(1, true);

            
            // Replace the selection with the modified text.

            selection.Text += "/**-------------------------------------------------------------------------------------------------------------------\n";

            CreateInfo(isGEN, name, description, group, ref info);
            selection.Text += info; 

            CreateCopyright(isGEN, ref copyright);
            selection.Text += copyright;
            
            selection.Text += "*---------------------------------------------------------------------------------------------------------------------*/\n";
            selection.Text += "\n";
            selection.Text += "/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/\n\n";
            selection.Text += "#include \"" + namewithoutext + ".h\"\n\n";
            selection.Text += "/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/\n\n";
            selection.Text += "/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/\n\n";            
        }

        private void ProcessHHeader(DTE dte, bool isGEN, string name, string ext, string group)
        {
            var     selection       = (TextSelection)dte.ActiveDocument.Selection;
            string  text            = selection.Text;
            var     namewithoutext  = Path.GetFileNameWithoutExtension(name);
            string  info            = "";
            string  copyright       = "";
            string  description     = Microsoft.VisualBasic.Interaction.InputBox(   "Purpose of the file" 	,
                                                                             	    "File header H,HPP"    ,
                                                                                    "");

            selection.GotoLine(1, true);

            // Replace the selection with the modified text.

            selection.Text += "/**-------------------------------------------------------------------------------------------------------------------\n";

            CreateInfo(isGEN, name, description, group, ref info);
            selection.Text += info;

            CreateCopyright(isGEN, ref copyright);
            selection.Text += copyright;

            selection.Text += "*---------------------------------------------------------------------------------------------------------------------*/\n";
            selection.Text += "\n";
            selection.Text += "#ifndef _" + (namewithoutext.ToUpper()) + "_" + ext.ToUpper() + "_" + "\n";
            selection.Text += "#define _" + (namewithoutext.ToUpper()) + "_" + ext.ToUpper() + "_" + "\n\n";

            selection.Text += "/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/\n\n";
            selection.Text += "/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/\n\n";
            selection.Text += "/*---- CLASS ---------------------------------------------------------------------------------------------------------*/\n\n";

          
            selection.Text += "\n\n";
            selection.Text += "class " + namewithoutext.ToUpper() + "\n";
            selection.Text += "{\n";            
            selection.Text += "\tpublic:\n";
            selection.Text += "\t\t\t\t"         + namewithoutext.ToUpper() + "\t\t()\t\t{ Clean();                            }\n";
            selection.Text += "\t\t\tvirtual  ~" + namewithoutext.ToUpper() + "\t\t()\t\t{ Clean();                            }\n";
            selection.Text += "\n";
            selection.Text += "\tprotected:\n";
            selection.Text += "\n";
            selection.Text += "\tprivate:\n";
            selection.Text += "\n";
            selection.Text += "\t\tvoid\t\tClean\t\t()\n";
            selection.Text += "\t\t\t\t{\n";
            selection.Text += "\n";
            selection.Text += "\t\t\t\t}\n";
            selection.Text += "};";
            selection.Text += "\n\n";
            selection.Text += "/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/\n\n";
            selection.Text += "#endif"; 
            selection.Text += "\n\n";
        }
    }
}
