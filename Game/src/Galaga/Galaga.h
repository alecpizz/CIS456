#pragma once
#include <Mage/Mage.h>


namespace Galaga
{
    class Galaga :
            public Mage::Application,
            public Mage::OnAppClosingEventListener
    {
    public:
        Galaga();
    private:
        void on_app_closing() override;
    };
}
