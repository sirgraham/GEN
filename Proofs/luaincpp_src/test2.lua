-- test script 2

function this.addme (this, x, y)
   return x + y
end

function this.subme (this, x, y)
   return x - y
end

function this.test (this)
   this:hello1 ()
   this:hello2 ()
   this:hello3 ()
end