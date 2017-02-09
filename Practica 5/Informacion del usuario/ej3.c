/**El bit setuid es asignable a ficheros ejecutables y permite que cuando
un usuario ejecute dicho fichero, el proceso adquiera los permisos del 
propietario del fichero ejecutado. EL ejemplo más claro del fichero ejecutable
con el bit setuid el su***/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main() {
	uid_t uid = getuid();
	uid_t euid = geteuid();
	printf("ID real: %i ID efectivo: %i\n",uid,euid);
	struct passwd *info_user = getpwuid(uid);
	struct passwd *info_userreal = getpwuid(euid);
	printf("Nombre del login: %s \nDirectorio Home: %s \n\n", info_user->pw_name, info_user->pw_dir);

	printf("Información del usuario real:\nID real: %i\nNombre del login: %s \nDirectorio Home: %s \n", info_userreal->pw_uid, info_userreal->pw_name, info_userreal->pw_dir); 


	return 0;
}
