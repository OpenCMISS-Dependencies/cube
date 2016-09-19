
:: 
:: This file is part of the CUBE software (http:\\www.scalasca.org\cube%
::
:: Copyright (c) 2009-2014, 
::    Forschungszentrum Juelich GmbH, Germany
::    German Research School for Simulation Sciences GmbH, Juelich\Aachen, Germany
::
:: Copyright (c) 2009-2012, 
::    RWTH Aachen University, Germany
::    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
::    Technische Universitaet Dresden, Germany
::    University of Oregon, Eugene, USA
::    Forschungszentrum Juelich GmbH, Germany
::    German Research School for Simulation Sciences GmbH, Juelich\Aachen, Germany
::    Technische Universitaet Muenchen, Germany
::
:: See the COPYING file in the package base directory for details.
::
:: maintainer Pavel Saviankou <p.saviankou@fz-juelich.de>

DEL /S /Q install

for %%f in ( 
cubew 
cube 
GUI-qt-core 
GUI-qt 
GUI-plugin-launch 
GUI-plugin-systemboxplot
GUI-plugin-systemtopology
tools\common 
tools\toolslibrary 
tools\calltree
tools\canonize
tools\clean
tools\cmp
tools\commoncalltree
tools\cube3to4
tools\cube4to3
tools\cut
tools\derive
tools\diff
tools\dump
tools\exclusify
tools\inclusify
tools\info
tools\isempty
tools\mean
tools\merge
tools\nodeview
tools\part
tools\rank
tools\regioninfo
tools\remap
tools\remap2
tools\sanity
tools\score
tools\stat
tools\tau2cube
tools\test
tools\topoassist
) do (
cd  %%f
DEL /S /Q Makefile*
rd /S /Q release
rd /S /Q debug
DEL /S /Q object_script*
cd %PWD%
)
