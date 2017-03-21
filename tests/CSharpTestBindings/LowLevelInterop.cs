using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpTestBindings
{
    public class LowLevelInterop
    {
    }

    public abstract class Animal
    {
        public abstract string GetName();
        public abstract string GetAction();
    }

    public abstract class Mamal : Animal
    {
    }

    public class Cat : Mamal
    {
        public Cat()
        {
            // create c++ cat
        }

        public override string GetAction()
        {
            throw new NotImplementedException();
        }

        public override string GetName()
        {
            throw new NotImplementedException();
        }
        // other stuff

    }
}
