// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDMM.h"

// Add default functionality here for any IDDMM functions that are not pure virtual.
void IDDMM::AssignModule(UDDModule* Mod)
{
	IModule = Mod;

	IDriver = UDDCommon::Get()->GetDriver();
}
