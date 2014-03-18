/* C:B**************************************************************************
This software is Copyright � 2014 Michael Romeo <r0m30@r0m30.com>

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

* C:E********************************************************************** */
/*
* scandisk (Windows Version)
* iterate through the disks attached to the system and return a sedDriveList
*/
/* ********************************************
* **THIS MODULE CONTAINS OS SPECIFIC CODE**
* ******************************************** */
#include "winos.h"
#include <stdlib.h>
#include <stdio.h>
#include "../scandisk.h"
#include "ioctoATA.h"
#include "../Discovery0.h"

driveList scandisk()
{
	driveList dl = 0;
	int i = 0;
	char * dev = (char *)malloc(25);
	HANDLE fh;
	while (true)
	{
		/*
		*  loop through the physical drives until we get a fail when we open it
		*/
		sprintf_s(dev, 23, "\\\\.\\PhysicalDrive%i", i);
		/* open the file handle so we can query it */
//		dev = "\\\\.\\PhysicalDrive3";	// limit this to a work drive for now
		fh = CreateFile((TCHAR *)dev,
			GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (INVALID_HANDLE_VALUE == fh) {
			DWORD err = GetLastError();
//			printf("error opening %s error code %d\n", dev, err);
			printf("Scan complete \n", dev, err);
			free(dev);
			break;
		}
		printf("Testing %s for TCG OPAL ... ", dev);
		PVOID buffer = (PVOID)malloc(512);
		if (NULL != buffer) {
			if (0x00 == ioctlATA(fh, IF_RECV, 0x01, 0x0100, buffer, 512))
			{
				printf("YES\n");
				new Discovery0(buffer);
			}
			else
				printf("NO\n");
			free(buffer);
		}
		CloseHandle(fh);
		i++;
	}
	return dl;
}