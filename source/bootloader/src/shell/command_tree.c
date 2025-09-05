#include <shell/command_tree.h>

#include <commands/echo.h>
#include <commands/exit.h>
#include <commands/boot.h>
#include <commands/clear.h>
#include <commands/color.h>
#include <commands/fibonacci.h>
#include <commands/calc.h>
#include <commands/lsmem.h>
#include <commands/gtn.h>
#include <commands/help.h>
#include <commands/version.h>
#include <commands/tree.h>
#include <commands/cat.h>
#include <commands/reset.h>
#include <commands/ls.h>
#include <commands/wmem.h>
#include <commands/stat.h>
#include <commands/touch.h>
#include <commands/af.h>
#include <commands/rm.h>
#include <commands/ed.h>
#include <commands/disc.h>
#include <commands/cp.h>
#include <commands/less.h>
#include <commands/lsdisc.h>

#define SHELL_COMMAND_TREE_ENTRY_NULL ((shell_command_tree_entry_t) { .command_ptr = NULL, .next_char = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, }, })

shell_command_tree_entry_t shell_command_tree_a;
shell_command_tree_entry_t shell_command_tree_af;

shell_command_tree_entry_t shell_command_tree_e;
shell_command_tree_entry_t shell_command_tree_ec;
shell_command_tree_entry_t shell_command_tree_ech;
shell_command_tree_entry_t shell_command_tree_echo;
shell_command_tree_entry_t shell_command_tree_ex;
shell_command_tree_entry_t shell_command_tree_exi;
shell_command_tree_entry_t shell_command_tree_exit;
shell_command_tree_entry_t shell_command_tree_ed;

shell_command_tree_entry_t shell_command_tree_b;
shell_command_tree_entry_t shell_command_tree_bo;
shell_command_tree_entry_t shell_command_tree_boo;
shell_command_tree_entry_t shell_command_tree_boot;

shell_command_tree_entry_t shell_command_tree_c;
shell_command_tree_entry_t shell_command_tree_cl;
shell_command_tree_entry_t shell_command_tree_cle;
shell_command_tree_entry_t shell_command_tree_clea;
shell_command_tree_entry_t shell_command_tree_clear;

shell_command_tree_entry_t shell_command_tree_co;
shell_command_tree_entry_t shell_command_tree_col;
shell_command_tree_entry_t shell_command_tree_colo;
shell_command_tree_entry_t shell_command_tree_color;

shell_command_tree_entry_t shell_command_tree_ca;
shell_command_tree_entry_t shell_command_tree_cal;
shell_command_tree_entry_t shell_command_tree_calc;

shell_command_tree_entry_t shell_command_tree_cat;

shell_command_tree_entry_t shell_command_tree_cp;

shell_command_tree_entry_t shell_command_tree_d;
shell_command_tree_entry_t shell_command_tree_di;
shell_command_tree_entry_t shell_command_tree_dis;
shell_command_tree_entry_t shell_command_tree_disc;

shell_command_tree_entry_t shell_command_tree_f;
shell_command_tree_entry_t shell_command_tree_fi;
shell_command_tree_entry_t shell_command_tree_fib;
shell_command_tree_entry_t shell_command_tree_fibo;
shell_command_tree_entry_t shell_command_tree_fibon;
shell_command_tree_entry_t shell_command_tree_fibona;
shell_command_tree_entry_t shell_command_tree_fibonac;
shell_command_tree_entry_t shell_command_tree_fibonacc;
shell_command_tree_entry_t shell_command_tree_fibonacci;

shell_command_tree_entry_t shell_command_tree_g;
shell_command_tree_entry_t shell_command_tree_gt;
shell_command_tree_entry_t shell_command_tree_gtn;

shell_command_tree_entry_t shell_command_tree_h;
shell_command_tree_entry_t shell_command_tree_he;
shell_command_tree_entry_t shell_command_tree_hel;
shell_command_tree_entry_t shell_command_tree_help;

shell_command_tree_entry_t shell_command_tree_l;
shell_command_tree_entry_t shell_command_tree_ls;
shell_command_tree_entry_t shell_command_tree_lsm;
shell_command_tree_entry_t shell_command_tree_lsme;
shell_command_tree_entry_t shell_command_tree_lsmem;

shell_command_tree_entry_t shell_command_tree_lsd;
shell_command_tree_entry_t shell_command_tree_lsdi;
shell_command_tree_entry_t shell_command_tree_lsdis;
shell_command_tree_entry_t shell_command_tree_lsdisc;

shell_command_tree_entry_t shell_command_tree_le;
shell_command_tree_entry_t shell_command_tree_les;
shell_command_tree_entry_t shell_command_tree_less;

shell_command_tree_entry_t shell_command_tree_v;
shell_command_tree_entry_t shell_command_tree_ve;
shell_command_tree_entry_t shell_command_tree_ver;
shell_command_tree_entry_t shell_command_tree_vers;
shell_command_tree_entry_t shell_command_tree_versi;
shell_command_tree_entry_t shell_command_tree_versio;
shell_command_tree_entry_t shell_command_tree_version;

shell_command_tree_entry_t shell_command_tree_t;
shell_command_tree_entry_t shell_command_tree_to;
shell_command_tree_entry_t shell_command_tree_tou;
shell_command_tree_entry_t shell_command_tree_touc;
shell_command_tree_entry_t shell_command_tree_touch;

shell_command_tree_entry_t shell_command_tree_tr;
shell_command_tree_entry_t shell_command_tree_tre;
shell_command_tree_entry_t shell_command_tree_tree;

shell_command_tree_entry_t shell_command_tree_r;
shell_command_tree_entry_t shell_command_tree_re;
shell_command_tree_entry_t shell_command_tree_res;
shell_command_tree_entry_t shell_command_tree_rese;
shell_command_tree_entry_t shell_command_tree_reset;

shell_command_tree_entry_t shell_command_tree_rm;

shell_command_tree_entry_t shell_command_tree_w;
shell_command_tree_entry_t shell_command_tree_wm;
shell_command_tree_entry_t shell_command_tree_wme;
shell_command_tree_entry_t shell_command_tree_wmem;

shell_command_tree_entry_t shell_command_tree_s;
shell_command_tree_entry_t shell_command_tree_st;
shell_command_tree_entry_t shell_command_tree_sta;
shell_command_tree_entry_t shell_command_tree_stat;

shell_command_tree_entry_t * shell_command_tree_root[26];

void shell_command_tree_init() {
    for (int i = 0; i < 26; i++) shell_command_tree_root[i] = NULL;
    shell_command_tree_e = SHELL_COMMAND_TREE_ENTRY_NULL;
    shell_command_tree_ec = SHELL_COMMAND_TREE_ENTRY_NULL;
    shell_command_tree_ech = SHELL_COMMAND_TREE_ENTRY_NULL;

    shell_command_tree_root['A' - 'A'] = &shell_command_tree_a;
    shell_command_tree_a.next_char['F' - 'A'] = &shell_command_tree_af;
    shell_command_tree_af.command_ptr = command_af;

    shell_command_tree_root['E' - 'A'] = &shell_command_tree_e;
    shell_command_tree_e.next_char['C' - 'A'] = &shell_command_tree_ec;
    shell_command_tree_ec.next_char['H' - 'A'] = &shell_command_tree_ech;
    shell_command_tree_ech.next_char['O' - 'A'] = &shell_command_tree_echo;
    shell_command_tree_echo.command_ptr = command_echo;
    shell_command_tree_e.next_char['X' - 'A'] = &shell_command_tree_ex;
    shell_command_tree_ex.next_char['I' - 'A'] = &shell_command_tree_exi;
    shell_command_tree_exi.next_char['T' - 'A'] = &shell_command_tree_exit;
    shell_command_tree_exit.command_ptr = command_exit;
    shell_command_tree_e.next_char['D' - 'A'] = &shell_command_tree_ed;
    shell_command_tree_ed.command_ptr = command_ed;

    shell_command_tree_root['B' - 'A'] = &shell_command_tree_b;
    shell_command_tree_b.next_char['O' - 'A'] = &shell_command_tree_bo;
    shell_command_tree_bo.next_char['O' - 'A'] = &shell_command_tree_boo;
    shell_command_tree_boo.next_char['T' - 'A'] = &shell_command_tree_boot;
    shell_command_tree_boot.command_ptr = command_boot;

    shell_command_tree_root['C' - 'A'] = &shell_command_tree_c;
    shell_command_tree_c.next_char['L' - 'A'] = &shell_command_tree_cl;
    shell_command_tree_cl.next_char['E' - 'A'] = &shell_command_tree_cle;
    shell_command_tree_cle.next_char['A' - 'A'] = &shell_command_tree_clea;
    shell_command_tree_clea.next_char['R' - 'A'] = &shell_command_tree_clear;
    shell_command_tree_clear.command_ptr = command_clear;

    shell_command_tree_c.next_char['O' - 'A'] = &shell_command_tree_co;
    shell_command_tree_co.next_char['L' - 'A'] = &shell_command_tree_col;
    shell_command_tree_col.next_char['O' - 'A'] = &shell_command_tree_colo;
    shell_command_tree_colo.next_char['R' - 'A'] = &shell_command_tree_color;
    shell_command_tree_color.command_ptr = command_color;

    shell_command_tree_c.next_char['A' - 'A'] = &shell_command_tree_ca;
    shell_command_tree_ca.next_char['L' - 'A'] = &shell_command_tree_cal;
    shell_command_tree_cal.next_char['C' - 'A'] = &shell_command_tree_calc;
    shell_command_tree_calc.command_ptr = command_calc;

    shell_command_tree_ca.next_char['T' - 'A'] = &shell_command_tree_cat;
    shell_command_tree_cat.command_ptr = command_cat;

    shell_command_tree_c.next_char['P' - 'A'] = &shell_command_tree_cp;
    shell_command_tree_cp.command_ptr = command_cp;

    shell_command_tree_root['D' - 'A'] = &shell_command_tree_d;
    shell_command_tree_d.next_char['I' - 'A'] = &shell_command_tree_di;
    shell_command_tree_di.next_char['S' - 'A'] = &shell_command_tree_dis;
    shell_command_tree_dis.next_char['C' - 'A'] = &shell_command_tree_disc;
    shell_command_tree_disc.command_ptr = command_disc;

    shell_command_tree_root['F' - 'A'] = &shell_command_tree_f;
    shell_command_tree_f.next_char['I' - 'A'] = &shell_command_tree_fi;
    shell_command_tree_fi.next_char['B' - 'A'] = &shell_command_tree_fib;
    shell_command_tree_fib.next_char['O' - 'A'] = &shell_command_tree_fibo;
    shell_command_tree_fibo.next_char['N' - 'A'] = &shell_command_tree_fibon;
    shell_command_tree_fibon.next_char['A' - 'A'] = &shell_command_tree_fibona;
    shell_command_tree_fibona.next_char['C' - 'A'] = &shell_command_tree_fibonac;
    shell_command_tree_fibonac.next_char['C' - 'A'] = &shell_command_tree_fibonacc;
    shell_command_tree_fibonacc.next_char['I' - 'A'] = &shell_command_tree_fibonacci;
    shell_command_tree_fibonacci.command_ptr = command_fibonacci;

    shell_command_tree_root['G' - 'A'] = &shell_command_tree_g;
    shell_command_tree_g.next_char['T' - 'A'] = &shell_command_tree_gt;
    shell_command_tree_gt.next_char['N' - 'A'] = &shell_command_tree_gtn;
    shell_command_tree_gtn.command_ptr = command_gtn;

    shell_command_tree_root['H' - 'A'] = &shell_command_tree_h;
    shell_command_tree_h.next_char['E' - 'A'] = &shell_command_tree_he;
    shell_command_tree_he.next_char['L' - 'A'] = &shell_command_tree_hel;
    shell_command_tree_hel.next_char['P' - 'A'] = &shell_command_tree_help;
    shell_command_tree_help.command_ptr = command_help;

    shell_command_tree_root['L' - 'A'] = &shell_command_tree_l;
    shell_command_tree_l.next_char['S' - 'A'] = &shell_command_tree_ls;
    shell_command_tree_ls.command_ptr = command_ls;
    shell_command_tree_ls.next_char['M' - 'A'] = &shell_command_tree_lsm;
    shell_command_tree_lsm.next_char['E' - 'A'] = &shell_command_tree_lsme;
    shell_command_tree_lsme.next_char['M' - 'A'] = &shell_command_tree_lsmem;
    shell_command_tree_lsmem.command_ptr = command_lsmem;

    shell_command_tree_ls.next_char['D' - 'A'] = &shell_command_tree_lsd;
    shell_command_tree_lsd.next_char['I' - 'A'] = &shell_command_tree_lsdi;
    shell_command_tree_lsdi.next_char['S' - 'A'] = &shell_command_tree_lsdis;
    shell_command_tree_lsdis.next_char['C' - 'A'] = &shell_command_tree_lsdisc;
    shell_command_tree_lsdisc.command_ptr = command_lsdisc;

    shell_command_tree_l.next_char['E' - 'A'] = &shell_command_tree_le;
    shell_command_tree_le.next_char['S' - 'A'] = &shell_command_tree_les;
    shell_command_tree_les.next_char['S' - 'A'] = &shell_command_tree_less;
    shell_command_tree_less.command_ptr = command_less;

    shell_command_tree_root['V' - 'A'] = &shell_command_tree_v;
    shell_command_tree_v.next_char['E' - 'A'] = &shell_command_tree_ve;
    shell_command_tree_ve.next_char['R' - 'A'] = &shell_command_tree_ver;
    shell_command_tree_ver.next_char['S' - 'A'] = &shell_command_tree_vers;
    shell_command_tree_vers.next_char['I' - 'A'] = &shell_command_tree_versi;
    shell_command_tree_versi.next_char['O' - 'A'] = &shell_command_tree_versio;
    shell_command_tree_versio.next_char['N' - 'A'] = &shell_command_tree_version;
    shell_command_tree_version.command_ptr = command_version;

    shell_command_tree_root['T' - 'A'] = &shell_command_tree_t;
    shell_command_tree_t.next_char['O' - 'A'] = &shell_command_tree_to;
    shell_command_tree_to.next_char['U' - 'A'] = &shell_command_tree_tou;
    shell_command_tree_tou.next_char['C' - 'A'] = &shell_command_tree_touc;
    shell_command_tree_touc.next_char['H' - 'A'] = &shell_command_tree_touch;
    shell_command_tree_touch.command_ptr = command_touch;

    shell_command_tree_t.next_char['R' - 'A'] = &shell_command_tree_tr;
    shell_command_tree_tr.next_char['E' - 'A'] = &shell_command_tree_tre;
    shell_command_tree_tre.next_char['E' - 'A'] = &shell_command_tree_tree;
    shell_command_tree_tree.command_ptr = command_tree;

    shell_command_tree_root['R' - 'A'] = &shell_command_tree_r;
    shell_command_tree_r.next_char['E' - 'A'] = &shell_command_tree_re;
    shell_command_tree_re.next_char['S' - 'A'] = &shell_command_tree_res;
    shell_command_tree_res.next_char['E' - 'A'] = &shell_command_tree_rese;
    shell_command_tree_rese.next_char['T' - 'A'] = &shell_command_tree_reset;
    shell_command_tree_reset.command_ptr = command_reset;

    shell_command_tree_r.next_char['M' - 'A'] = &shell_command_tree_rm;
    shell_command_tree_rm.command_ptr = command_rm;

    shell_command_tree_root['W' - 'A'] = &shell_command_tree_w;
    shell_command_tree_w.next_char['M' - 'A'] = &shell_command_tree_wm;
    shell_command_tree_wm.next_char['E' - 'A'] = &shell_command_tree_wme;
    shell_command_tree_wme.next_char['M' - 'A'] = &shell_command_tree_wmem;
    shell_command_tree_wmem.command_ptr = command_wmem;

    shell_command_tree_root['S' - 'A'] = &shell_command_tree_s;
    shell_command_tree_s.next_char['T' - 'A'] = &shell_command_tree_st;
    shell_command_tree_st.next_char['A' - 'A'] = &shell_command_tree_sta;
    shell_command_tree_sta.next_char['T' - 'A'] = &shell_command_tree_stat;
    shell_command_tree_stat.command_ptr = command_stat;
}