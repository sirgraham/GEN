
-- Tests the parameter passing, using and returning
function this.divideMe (this, count)
   trace ("In divideMe...");
   return count / 30
end

-- Tests lua calling lua functions
function this.luaCallMe ()
   trace( "luaCallMe" )
end

function this.luaCallMeToo (this)
   this:hello1 ()
end

-- Main test routing
function this.CountAndCall (this, i)
   this:luaCallMe ()
   this:luaCallMeToo ()
   trace ("CountAndCall")

   while  (i <= 4) do
      this:hello1 ()
      this:hello2 (i)
      i = i+1
   end

   trace ("Hello, debug world!")
end

-- Tests the C-Api returning values to lua
function this.returnToMe (this, i)
   trace ("returnToMe")
   x = this:hello3 (i)
   io.write ("returnToMe ", x)
end