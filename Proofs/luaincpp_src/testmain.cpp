#include "luavirtualmachine.h"
#include "luascript.h"
#include "luadebugger.h"
#include "lualib/luainc.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


class CMyScript : public CLuaScript
{
public:
   CMyScript (CLuaVirtualMachine& vm) : CLuaScript (vm)
   {
      m_iMethodBase = RegisterFunction ("hello1");
      RegisterFunction ("hello2");
      RegisterFunction ("hello3");
   }

   int ScriptCalling (CLuaVirtualMachine& vm, int iFunctionNumber)
   {
      switch (iFunctionNumber - m_iMethodBase)
      {
      case 0:
         return Hello1 (vm);
      case 1:
         return Hello2 (vm);
      case 2:
         return Hello3 (vm);
      }

      return 0;
   }

   int Hello1 (CLuaVirtualMachine& vm)
   {
      printf ("Hellow (1)\n");
      return 0;
   }

   int Hello2 (CLuaVirtualMachine& vm)
   {
      lua_State *state = (lua_State *) vm;

      int iNumber = (int) lua_tonumber (state, -1);
      printf ("Hellow (2) -> %d\n", iNumber);
      return 0;
   }

   int Hello3 (CLuaVirtualMachine& vm)
   {
      lua_State *state = (lua_State *) vm;

      int iNumber = (int) lua_tonumber (state, -1);
      printf ("Hello (3) -> %d\n", iNumber);
      lua_pushnumber (state, iNumber + 2);
      return 1;
   }

   void HandleReturns (CLuaVirtualMachine& vm, const char *strFunc)
   {
      if (strcmp (strFunc, "divideMe") == 0)
      {
         // frames returns an answer of the stack
         lua_State *state = (lua_State *) vm;
         int itop = lua_gettop (state);
         int iType = lua_type (state, -1);
         const char *s = lua_typename (state, iType);
         double fFrameCount = lua_tonumber (state, -1);
         
         printf ("frame count = %f\n", fFrameCount);
      }
   }

protected:
   int m_iMethodBase;
};

class CMyScript2 : public CLuaScript
{
public:
   CMyScript2 (CLuaVirtualMachine& vm) : CLuaScript (vm)
   {
      m_iMethodBase = RegisterFunction ("hello1");
      RegisterFunction ("hello2");
      RegisterFunction ("hello3");
   }

   int Hello1 (CLuaVirtualMachine& vm)
   {
      printf ("Hello2 (1)\n");
      return 0;
   }

   int Hello2 (CLuaVirtualMachine& vm)
   {
      printf ("Hello2 (2)\n");
      return 0;
   }

   int Hello3 (CLuaVirtualMachine& vm)
   {
      printf ("Hello2 (3)\n");
      return 0;
   }

   int ScriptCalling (CLuaVirtualMachine& vm, int iFunctionNumber)
   {
      switch (iFunctionNumber - m_iMethodBase)
      {
      case 0:
         return Hello1 (vm);
      case 1:
         return Hello2 (vm);
      case 2:
         return Hello3 (vm);
      }

      return 0;
   }

   void HandleReturns (CLuaVirtualMachine& vm, const char *strFunc)
   {
      if (strcmp (strFunc, "addme") == 0)
      {
         // frames returns an answer of the stack
         lua_State *state = (lua_State *) vm;
         
         double addres = lua_tonumber (state, -1);
         
         printf ("add res = %f\n", addres);
      }
      else if (strcmp (strFunc, "subme") == 0)
      {
         lua_State *state = (lua_State *) vm;
         
         double addres = lua_tonumber (state, -1);
         
         printf ("sub res = %f\n", addres);
      }
   }

protected:
   int m_iMethodBase;

};


void main (void)
{
   CLuaVirtualMachine vm;
   vm.InitialiseVM ();

   CLuaDebugger dbg (vm);
   dbg.SetCount (10);
   //dbg.SetHooksFlag (DBG_MASK_LINE);
   
   CMyScript ms (vm);
   ms.CompileFile ("test.lua");

   CMyScript2 ms2 (vm);
   ms2.CompileFile ("test2.lua");

   int iTopS = lua_gettop ((lua_State *) vm);

   assert (ms.ScriptHasFunction ("CountAndCall"));
   assert (!ms.ScriptHasFunction ("countAndCall"));
   assert (!ms.ScriptHasFunction ("test"));

   ms.SelectScriptFunction ("CountAndCall");
   ms.AddParam (2);
   ms.Go ();

   ms.SelectScriptFunction ("divideMe");
   ms.AddParam (33);
   ms.Go (1);

   ms.SelectScriptFunction ("returnToMe");
   ms.AddParam (10);
   ms.Go (0);

   int iTopE = lua_gettop ((lua_State *) vm);

   if (iTopS != iTopE)
   {
      printf ("ERROR: Stack Not Balanced!");
   }

   ms2.SelectScriptFunction ("test");
   ms2.Go ();

   ms2.SelectScriptFunction ("addme");
   ms2.AddParam (2);
   ms2.AddParam (3);
   ms2.Go (1);

   ms2.SelectScriptFunction ("subme");
   ms2.AddParam (4);
   ms2.AddParam (2);
   ms2.Go (1);
}