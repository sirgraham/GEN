//------------------------------------------------------------------------------
// <copyright file="FunctionHeader.cs" company="GEN Group">
//   Copyright (c) GEN Group.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.ComponentModel.Design;
using System.Globalization;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using EnvDTE;
using System.IO;

namespace Extensions
{
    /// <summary>
    /// Command handler
    /// </summary>
    internal sealed class FunctionHeader
    {
        string author     = "";
        string enterprise = "";

        /// <summary>
        /// Command ID.
        /// </summary>
        public const int CommandId = 0x0100;

        /// <summary>
        /// Command menu group (command set GUID).
        /// </summary>
        public static readonly Guid CommandSet = new Guid("f1afa488-901c-4e32-968e-37155ebb3d2e");

        /// <summary>
        /// VS Package that provides this command, not null.
        /// </summary>
        private readonly Package package;

        /// <summary>
        /// Initializes a new instance of the <see cref="FunctionHeader"/> class.
        /// Adds our command handlers for menu (commands must exist in the command table file)
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        private FunctionHeader(Package package)
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

                DTE dte = (DTE)ServiceProvider.GetService(typeof(DTE));
                string userfoldername = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);

                try
                {
                    
                    using (StreamReader sr = new StreamReader(userfoldername + "\\MacrosAuthor.txt"))
                    {
                        String line = sr.ReadToEnd();
                        string[] array = line.Split('\n');
                        author      = array[0].Replace('\r', ' ');                        
                        enterprise  = array[1].Replace('\r', ' ');
                    }
                }
                catch (Exception)
                {
                    author = Microsoft.VisualBasic.Interaction.InputBox(
                        "Names",
                        "Author name",
                        "");

                    if (author.Length == 0)
                        return;

                    enterprise = Microsoft.VisualBasic.Interaction.InputBox(
                        "Names",
                        "Enterprise name",
                        "");

                    if (enterprise.Length == 0)
                        return;

                    using (StreamWriter sw = new StreamWriter(userfoldername + "\\MacrosAuthor.txt"))
                    {
                        sw.WriteLine(author);
                        sw.WriteLine(enterprise);
                        sw.Close();
                    }
                      
                }
            }
        }

        /// <summary>
        /// Gets the instance of the command.
        /// </summary>
        public static FunctionHeader Instance
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
            Instance = new FunctionHeader(package);
        }

        private void GetGroupID(string directory, ref bool isGEN, ref string groupID)
        {
            // is GEN?
            if (directory.Contains("GENFrameWork")) isGEN = true;

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

            if(dte.ActiveDocument != null)
              {
                var selection       = (TextSelection)dte.ActiveDocument.Selection;
                string text         = selection.Text;
                         
                string funcname     = "";
                string description  = "";
                
                string version      = "";
                string returntype   = "";

                string[] parameters = null;

                DateTime localDate  = DateTime.Now;
                var      culture    = new CultureInfo("es-ES");

                version =localDate.ToString(culture);

                if(text.Length==0) return;

                //------------ eliminamos tabs y espacios

                string line = text.Replace("\t", " ");
                while (line.IndexOf(" *") > 0)
                  {
                    line = line.Replace(" *", "*");
                  }

                while(line.IndexOf("  ") > 0)
                  {
                    line = line.Replace("  ", " ");
                  }

                line.Trim();
                text = line;

                if(text.Length == 0) return;

                //------------ buscamos el return type y el contexto
                int parenthesis = text.IndexOf('(');
                int space = text.IndexOf(' ');
                int i;
                if((space == -1) || (parenthesis < space)) //no tiene return type (constructor)
                  {
                    returntype = "";

                    if(text.IndexOf('~') == -1)
                           description = "Constructor";
                      else description = "Destructor";

                    if(parenthesis < 0) return;

                    if(parenthesis >= text.Length) return;

                    funcname = text.Substring(0, parenthesis);
                    parameters = text.Substring(parenthesis + 1, text.Length - parenthesis - 2).Split(',');

                    for(i = 0; i < parameters.Length; i++)
                       {
                         int index=parameters[i].IndexOf(' ');

                         if (index>0) parameters[i] = parameters[i].Substring(0, index);
                       }
                  }
             
                if((space != -1) && (parenthesis > space))
                  {
                    description = Microsoft.VisualBasic.Interaction.InputBox( "Purpose of this function"  ,
                    	                                                      "Function of file" 					,
                                                                              "");

                    returntype = text.Substring(0, space);
                    funcname   = text.Substring(space, parenthesis-space);

                    parameters = text.Substring(parenthesis + 1, text.Length - parenthesis-2).Split(',');
                                        
                    for(i=0; i<parameters.Length; i++)
                       {
                         parameters[i] = parameters[i].Trim();

                        int size = parameters[i].Length;
                        int pos = parameters[i].IndexOf(' ');

                        if(pos != -1)
                          {
                            if(size > pos + 1)
                              {
                                parameters[i] = parameters[i].Substring(pos + 1, (size-(pos+1)));
                              }
                          }
                      }
                  }

                var     dtename     = dte.ActiveDocument.FullName;
                string  date        = DateTime.Now.ToString();
                string  directory   = "";
                bool    isGEN       = false;
                string  group       = "";
                
                directory = Path.GetDirectoryName(dtename);

                GetGroupID(directory, ref isGEN, ref group);

                string result;

                result  = "/**-------------------------------------------------------------------------------------------------------------------\n";
                result += "*\n";
                result += "*\t@fn \t\t\t\t"    + funcname.TrimStart() + "\n";
                result += "*\t@brief\t\t\t"    + description + "\n";                
                result += "*\t@ingroup\t\t"    + group       + "\n";  
                result += "*\n";
                result += "*\t@author \t\t"    + author      + "\n";       
                result += "*\t@date\t\t\t\t"   + date        + "\n";
              
                
                if(parameters != null)
                  {
                    if(parameters.Length != 0)
                      {
                        result += "*\n";
                      }
                    
                    for (i = 0; i < parameters.Length; i++)
                      {
                        if(parameters[i].Length != 0)
                               result += "*\t@param[]\t\t" + parameters[i] + " : \n";
                          else result += "*\t@param\n";
                      }
                  }
                  
                if(returntype.Length == 0)
                  {
                     result += "*\t@return\n";       
                  }           
                 else
                  {
                    result += "*\n";
                    if(returntype == "void")
                      {
                        result += "*\t@return\t\t\t" + returntype + " : does not return anything. \n";    
                      }
                     else
                      { 
                        if(returntype == "bool")
                          {
                            result += "*\t@return\t\t\t" + returntype + " : true if is succesful. \n";    
                          }
                         else
                          {
                            result += "*\t@return\t\t\t" + returntype + " : \n";
                          }
                      }
                  }

                result += "*\n";
                result += "*---------------------------------------------------------------------------------------------------------------------*/\n";
                result += selection.Text;

                selection.Text = result;
            }
        }
    }
}
