#include "Ycontroller.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void Ycontroller::config(TouchScreen& _screen, PID& _PIDx, PID& _PIDy, PID& _outerPIDx, PID& _outerPIDy)
{
	screen 			= &_screen;
	ctrlx				= &_PIDx;
	ctrly				= &_PIDy;
	outerCtrlx	= &_outerPIDx;
	outerCtrly 	= &_outerPIDy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::begin()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::reset()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::process(float timestep)
{
	//if (outerControlFlag)
	//{
	//	internalXDesired = outerCtrlx->compute(xDesired,screen->getX(),timestep,screen->getDX());
	//	internalYDesired = outerCtrly->compute(yDesired,screen->getY(),timestep,screen->getDY());
	//	ux = ctrlx->compute(internalXDesired,screen->getX(),timestep,screen->getDX());
	//	uy = ctrly->compute(internalYDesired,screen->getY(),timestep,screen->getDY());
	//}
	//else
	//{
		//ux = ctrlx->compute(xDesired,screen->getX(),timestep,screen->getDX());
		ux = ctrlx->testCompute(xDesired, screen->getX(), timestep);
		uy = ctrly->testCompute(yDesired, screen->getY(), timestep);
		//uy = ctrly->compute(yDesired,screen->getY(),timestep,screen->getDY());
	/*}

	if (internalXDesired > 200 || internalXDesired < 200)
	{
		outerCtrlx->reset();
	}

	if (internalYDesired > 200 || internalYDesired < 200)
	{
		outerCtrly->reset();
	}


	if (screen->m_noTouchFlag || screen->m_resetFlag)
	{
		outerCtrlx->reset();
		outerCtrly->reset();
		ctrlx->reset();
		ctrly->reset();
		screen->m_resetFlag = false;
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
