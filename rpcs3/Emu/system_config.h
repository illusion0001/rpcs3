#pragma once

#include "system_config_types.h"
#include "Utilities/Config.h"

enum CellNetCtlState : s32;
enum CellSysutilLicenseArea : s32;
enum CellSysutilLang : s32;
enum CellKbMappingType : s32;

struct cfg_root : cfg::node
{
	struct node_core : cfg::node
	{
	private:
		/** We don't wanna include the sysinfo header here */
		static bool enable_tsx_by_default();

	public:
		node_core(cfg::node* _this) : cfg::node(_this, "Core") {}

		cfg::_enum<ppu_decoder_type> ppu_decoder{ this, "PPU Decoder", ppu_decoder_type::llvm };
		cfg::_int<1, 8> ppu_threads{ this, "PPU Threads", 2 }; // Amount of PPU threads running simultaneously (must be 2)
		cfg::_bool ppu_debug{ this, "PPU Debug" };
		cfg::_bool ppu_call_history{ this, "PPU Calling History" }; // Enable PPU calling history recording
		cfg::_bool llvm_logs{ this, "Save LLVM logs" };
		cfg::string llvm_cpu{ this, "Use LLVM CPU" };
		cfg::_int<0, 1024> llvm_threads{ this, "Max LLVM Compile Threads", 0 };
		cfg::_bool ppu_llvm_greedy_mode{ this, "PPU LLVM Greedy Mode", false, false };
		cfg::_bool ppu_llvm_precompilation{ this, "PPU LLVM Precompilation", true };
		cfg::_enum<thread_scheduler_mode> thread_scheduler{this, "Thread Scheduler Mode", thread_scheduler_mode::os};
		cfg::_bool set_daz_and_ftz{ this, "Set DAZ and FTZ", false };
		cfg::_enum<spu_decoder_type> spu_decoder{ this, "SPU Decoder", spu_decoder_type::llvm };
		cfg::uint<0, 100> spu_reservation_busy_waiting_percentage{ this, "SPU Reservation Busy Waiting Percentage", 0, true };
		cfg::uint<0, 100> spu_getllar_busy_waiting_percentage{ this, "SPU GETLLAR Busy Waiting Percentage", 100, true };
		cfg::_bool spu_debug{ this, "SPU Debug" };
		cfg::_bool mfc_debug{ this, "MFC Debug" };
		cfg::_int<0, 6> preferred_spu_threads{ this, "Preferred SPU Threads", 0, true }; // Number of hardware threads dedicated to heavy simultaneous spu tasks
		cfg::_int<0, 16> spu_delay_penalty{ this, "SPU delay penalty", 3 }; // Number of milliseconds to block a thread if a virtual 'core' isn't free
		cfg::_bool spu_loop_detection{ this, "SPU loop detection", false, true }; // Try to detect wait loops and trigger thread yield
		cfg::_int<0, 6> max_spurs_threads{ this, "Max SPURS Threads", 6 }; // HACK. If less then 6, max number of running SPURS threads in each thread group.
		cfg::_enum<spu_block_size_type> spu_block_size{ this, "SPU Block Size", spu_block_size_type::safe };
		cfg::_bool spu_accurate_getllar{ this, "Accurate GETLLAR", false, true };
		cfg::_bool spu_accurate_dma{ this, "Accurate SPU DMA", false };
		cfg::_bool spu_accurate_reservations{ this, "Accurate SPU Reservations", true };
		cfg::_bool accurate_cache_line_stores{ this, "Accurate Cache Line Stores", false };
		cfg::_bool rsx_accurate_res_access{this, "Accurate RSX reservation access", false, true};

		struct fifo_setting : public cfg::_enum<rsx_fifo_mode>
		{
			using _enum = cfg::_enum<rsx_fifo_mode>;
			using _enum::_enum;

			explicit operator bool() const
			{
				return get() != rsx_fifo_mode::fast;
			}
		};

		fifo_setting rsx_fifo_accuracy{this, "RSX FIFO Accuracy", rsx_fifo_mode::fast };
		cfg::_bool spu_verification{ this, "SPU Verification", true }; // Should be enabled
		cfg::_bool spu_cache{ this, "SPU Cache", true };
		cfg::_bool spu_prof{ this, "SPU Profiler", false };
		cfg::uint<0, 16> mfc_transfers_shuffling{ this, "MFC Commands Shuffling Limit", 0 };
		cfg::uint<0, 10000> mfc_transfers_timeout{ this, "MFC Commands Timeout", 0, true };
		cfg::_bool mfc_shuffling_in_steps{ this, "MFC Commands Shuffling In Steps", false, true };
		cfg::_enum<tsx_usage> enable_TSX{ this, "Enable TSX", enable_tsx_by_default() ? tsx_usage::enabled : tsx_usage::disabled }; // Enable TSX. Forcing this on Haswell/Broadwell CPUs should be used carefully
		cfg::_bool spu_accurate_xfloat{ this, "Accurate xfloat", false };
		cfg::_bool spu_approx_xfloat{ this, "Approximate xfloat", true };
		cfg::_bool spu_relaxed_xfloat{ this, "Relaxed xfloat", true }; // Approximate accuracy for only the "FCGT" and "FNMS" instructions
		cfg::_int<-1, 14> ppu_128_reservations_loop_max_length{ this, "Accurate PPU 128-byte Reservation Op Max Length", 0, true }; // -1: Always accurate, 0: Never accurate, 1-14: max accurate loop length
		cfg::_int<-64, 64> stub_ppu_traps{ this, "Stub PPU Traps", 0, true }; // Hack, skip PPU traps for rare cases where the trap is continueable (specify relative instructions to skip)
		cfg::_bool full_width_avx512{ this, "Full Width AVX-512", false };
		cfg::_bool ppu_llvm_nj_fixup{ this, "PPU LLVM Java Mode Handling", true }; // Partially respect current Java Mode for alti-vec ops by PPU LLVM
		cfg::_bool use_accurate_dfma{ this, "Use Accurate DFMA", true }; // Enable accurate double-precision FMA for CPUs which do not support it natively
		cfg::_bool ppu_set_sat_bit{ this, "PPU Set Saturation Bit", false }; // Accuracy. If unset, completely disable saturation flag handling.
		cfg::_bool ppu_use_nj_bit{ this, "PPU Accurate Non-Java Mode", false }; // Accuracy. If set, accurately emulate NJ flag. Implies NJ fixup.
		cfg::_bool ppu_fix_vnan{ this, "PPU Fixup Vector NaN Values", false }; // Accuracy. Partial.
		cfg::_bool ppu_set_vnan{ this, "PPU Accurate Vector NaN Values", false }; // Accuracy. Implies ppu_fix_vnan.
		cfg::_bool ppu_set_fpcc{ this, "PPU Set FPCC Bits", false }; // Accuracy.

		cfg::_enum<spu_instruction_accuracy> spu_fcgt_accuracy{this, "FCGT accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fcmgt_accuracy{this, "FCMGT accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fm_accuracy{this, "FM accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fnms_accuracy{this, "FNMS accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fma_accuracy{this, "FMA accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fms_accuracy{this, "FMS accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_frest_accuracy{this, "FREST accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_frsqest_accuracy{this, "FRSQEST accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fi_accuracy{this, "FI accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fa_accuracy{this, "FA accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fs_accuracy{this, "FS accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fesd_accuracy{this, "FESD accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_frds_accuracy{this, "FRDS accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fceq_accuracy{this, "FCEQ accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_fcmeq_accuracy{this, "FCMEQ accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_cflts_accuracy{this, "CFLTS accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_cfltu_accuracy{this, "CFLTU accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_csflt_accuracy{this, "CSFLT accuracy", spu_instruction_accuracy::Default};
		cfg::_enum<spu_instruction_accuracy> spu_cuflt_accuracy{this, "CUFLT accuracy", spu_instruction_accuracy::Default};
		cfg::_bool spu_gta4_FM{this, "GTAIV FM", false};
		cfg::_bool debug_console_mode{ this, "Debug Console Mode", false }; // Debug console emulation, not recommended
		cfg::_bool hook_functions{ this, "Hook static functions" };
		cfg::set_entry libraries_control{ this, "Libraries Control" }; // Override HLE/LLE behaviour of selected libs
		cfg::_bool hle_lwmutex{ this, "HLE lwmutex" }; // Force alternative lwmutex/lwcond implementation
		cfg::uint64 spu_llvm_lower_bound{ this, "SPU LLVM Lower Bound" };
		cfg::uint64 spu_llvm_upper_bound{ this, "SPU LLVM Upper Bound", 0xffffffffffffffff };
		cfg::uint64 tx_limit1_ns{this, "TSX Transaction First Limit", 800}; // In nanoseconds
		cfg::uint64 tx_limit2_ns{this, "TSX Transaction Second Limit", 2000}; // In nanoseconds

		cfg::_int<10, 3000> clocks_scale{ this, "Clocks scale", 100 }; // Changing this from 100 (percentage) may affect game speed in unexpected ways
		cfg::uint<0, 3000> spu_wakeup_delay{ this, "SPU Wake-Up Delay", 0, true };
		cfg::uint<0, (1 << 6) - 1> spu_wakeup_delay_mask{ this, "SPU Wake-Up Delay Thread Mask", (1 << 6) - 1, true };
		cfg::uint<0, 400> max_cpu_preempt_count_per_frame{ this, "Max CPU Preempt Count", 0, true }; 
		cfg::_bool allow_rsx_cpu_preempt{ this, "Allow RSX CPU Preemptions", true, true }; 
#if defined (__linux__) || defined (__APPLE__)
		cfg::_enum<sleep_timers_accuracy_level> sleep_timers_accuracy{ this, "Sleep Timers Accuracy", sleep_timers_accuracy_level::_as_host, true };
#else
		cfg::_enum<sleep_timers_accuracy_level> sleep_timers_accuracy{ this, "Sleep Timers Accuracy", sleep_timers_accuracy_level::_usleep, true };
#endif

		cfg::uint64 perf_report_threshold{this, "Performance Report Threshold", 500, true}; // In µs, 0.5ms = default, 0 = everything
		cfg::_bool perf_report{this, "Enable Performance Report", false, true}; // Show certain perf-related logs
		cfg::_bool external_debugger{this, "Assume External Debugger"};
	} core{ this };

	struct node_vfs : cfg::node
	{
		node_vfs(cfg::node* _this) : cfg::node(_this, "VFS") {}

		cfg::_bool host_root{ this, "Enable /host_root/" };
		cfg::_bool init_dirs{ this, "Initialize Directories", true };

		cfg::_bool limit_cache_size{ this, "Limit disk cache size", false };
		cfg::_int<0, 10240> cache_max_size{ this, "Disk cache maximum size (MB)", 5120 };
		cfg::_bool empty_hdd0_tmp{ this, "Empty /dev_hdd0/tmp/", true };

	} vfs{ this };

	struct node_video : cfg::node
	{
		node_video(cfg::node* _this) : cfg::node(_this, "Video") {}

#ifdef __APPLE__
		cfg::_enum<video_renderer> renderer{ this, "Renderer", video_renderer::vulkan };
#else
		cfg::_enum<video_renderer> renderer{ this, "Renderer", video_renderer::opengl };
#endif

		cfg::_enum<video_resolution> resolution{ this, "Resolution", video_resolution::_720 };
		cfg::_enum<video_aspect> aspect_ratio{ this, "Aspect ratio", video_aspect::_16_9 };
		cfg::_enum<frame_limit_type> frame_limit{ this, "Frame limit", frame_limit_type::_auto, true };
		cfg::_float<0, 1000> second_frame_limit{ this, "Second Frame Limit", 0, true }; // 0 disables its effect
		cfg::_enum<msaa_level> antialiasing_level{ this, "MSAA", msaa_level::_auto };
		cfg::_enum<shader_mode> shadermode{ this, "Shader Mode", shader_mode::async_recompiler };
		cfg::_enum<gpu_preset_level> shader_precision{ this, "Shader Precision", gpu_preset_level::high };

		cfg::_bool write_color_buffers{ this, "Write Color Buffers" };
		cfg::_bool write_depth_buffer{ this, "Write Depth Buffer" };
		cfg::_bool read_color_buffers{ this, "Read Color Buffers" };
		cfg::_bool read_depth_buffer{ this, "Read Depth Buffer" };
		cfg::_bool log_programs{ this, "Log shader programs" };
		cfg::_bool vsync{ this, "VSync" };
		cfg::_bool debug_output{ this, "Debug output" };
		cfg::_bool overlay{ this, "Debug overlay", false, true };
		cfg::_bool renderdoc_compatiblity{ this, "Renderdoc Compatibility Mode" };
		cfg::_bool use_gpu_texture_scaling{ this, "Use GPU texture scaling", false };
		cfg::_bool reprotect_hack{ this, "Reprotect Hack", false }; // Workaround for flickering shadows and missing water
		cfg::_bool vk_event_hack{ this, "VK Event Hack", false };
		cfg::_bool stretch_to_display_area{ this, "Stretch To Display Area", false, true };
		cfg::_bool force_high_precision_z_buffer{ this, "Force High Precision Z buffer" };
		cfg::_bool strict_rendering_mode{ this, "Strict Rendering Mode" };
		cfg::_bool disable_zcull_queries{ this, "Disable ZCull Occlusion Queries", false, true };
		cfg::_bool disable_video_output{ this, "Disable Video Output", false, true };
		cfg::_bool disable_vertex_cache{ this, "Disable Vertex Cache", false };
		cfg::_bool disable_FIFO_reordering{ this, "Disable FIFO Reordering", false };
		cfg::_bool frame_skip_enabled{ this, "Enable Frame Skip", false, true };
		cfg::_bool force_cpu_blit_processing{ this, "Force CPU Blit", false, true }; // Debugging option
		cfg::_bool disable_on_disk_shader_cache{ this, "Disable On-Disk Shader Cache", false };
		cfg::_bool disable_vulkan_mem_allocator{ this, "Disable Vulkan Memory Allocator", false };
		cfg::_bool full_rgb_range_output{ this, "Use full RGB output range", true, true }; // Video out dynamic range
		cfg::_bool strict_texture_flushing{ this, "Strict Texture Flushing", false };
		cfg::_bool multithreaded_rsx{ this, "Multithreaded RSX", false };
		cfg::_int<0, 0xFFFFFFFFu> zcull_default_value{this, "ZCull query default value", 1};
		cfg::_bool relaxed_zcull_sync{ this, "Relaxed ZCULL Sync", false };
		cfg::_bool enable_3d{ this, "Enable 3D", false };
		cfg::_bool debug_program_analyser{ this, "Debug Program Analyser", false };
		cfg::_bool precise_zpass_count{ this, "Accurate ZCULL stats", true };
		cfg::_int<1, 8> consecutive_frames_to_draw{ this, "Consecutive Frames To Draw", 1, true};
		cfg::_int<1, 8> consecutive_frames_to_skip{ this, "Consecutive Frames To Skip", 1, true};
		cfg::_int<50, 800> resolution_scale_percent{ this, "Resolution Scale", 100 };
		cfg::uint<0, 16> anisotropic_level_override{ this, "Anisotropic Filter Override", 0, true };
		cfg::_float<-32, 32> texture_lod_bias{ this, "Texture LOD Bias Addend", 0, true };
		cfg::_int<1, 1024> min_scalable_dimension{ this, "Minimum Scalable Dimension", 16 };
		cfg::_int<0, 16> shader_compiler_threads_count{ this, "Shader Compiler Threads", 0 };
		cfg::_int<0, 30000000> driver_recovery_timeout{ this, "Driver Recovery Timeout", 1000000, true };
		cfg::uint<0, 16667> driver_wakeup_delay{ this, "Driver Wake-Up Delay", 1, true };
		cfg::_int<1, 1800> vblank_rate{ this, "Vblank Rate", 60, true }; // Changing this from 60 may affect game speed in unexpected ways
		cfg::_bool vblank_ntsc{ this, "Vblank NTSC Fixup", false, true };
		cfg::_bool decr_memory_layout{ this, "DECR memory layout", false}; // Force enable increased allowed main memory range as DECR console
		cfg::_bool host_label_synchronization{ this, "Allow Host GPU Labels", false };
		cfg::_bool disable_msl_fast_math{ this, "Disable MSL Fast Math", false };

		struct node_vk : cfg::node
		{
			node_vk(cfg::node* _this) : cfg::node(_this, "Vulkan") {}

			cfg::string adapter{ this, "Adapter" };
			cfg::_bool force_fifo{ this, "Force FIFO present mode" };
			cfg::_bool force_primitive_restart{ this, "Force primitive restart flag" };
			cfg::_enum<vk_exclusive_fs_mode> exclusive_fullscreen_mode{ this, "Exclusive Fullscreen Mode", vk_exclusive_fs_mode::unspecified};
			cfg::_bool asynchronous_texture_streaming{ this, "Asynchronous Texture Streaming 2", false };
			cfg::_bool fsr_upscaling{ this, "Enable FidelityFX Super Resolution Upscaling", false, true };
			cfg::uint<0, 100> rcas_sharpening_intensity{ this, "FidelityFX CAS Sharpening Intensity", 50, true };
			cfg::_enum<vk_gpu_scheduler_mode> asynchronous_scheduler{ this, "Asynchronous Queue Scheduler", vk_gpu_scheduler_mode::safe };

		} vk{ this };

		struct node_perf_overlay : cfg::node
		{
			node_perf_overlay(cfg::node* _this) : cfg::node(_this, "Performance Overlay") {}

			cfg::_bool perf_overlay_enabled{ this, "Enabled", false, true };
			cfg::_bool framerate_graph_enabled{ this, "Enable Framerate Graph", false, true };
			cfg::_bool frametime_graph_enabled{ this, "Enable Frametime Graph", false, true };
			cfg::uint<2, 6000> framerate_datapoint_count{ this, "Framerate datapoints", 50, true };
			cfg::uint<2, 6000> frametime_datapoint_count{ this, "Frametime datapoints", 170, true };
			cfg::_enum<detail_level> level{ this, "Detail level", detail_level::medium, true };
			cfg::_enum<perf_graph_detail_level> framerate_graph_detail_level{ this, "Framerate graph detail level", perf_graph_detail_level::show_all, true };
			cfg::_enum<perf_graph_detail_level> frametime_graph_detail_level{ this, "Frametime graph detail level", perf_graph_detail_level::show_all, true };
			cfg::uint<1, 1000> update_interval{ this, "Metrics update interval (ms)", 350, true };
			cfg::uint<4, 36> font_size{ this, "Font size (px)", 10, true };
			cfg::_enum<screen_quadrant> position{ this, "Position", screen_quadrant::top_left, true };
			cfg::string font{ this, "Font", "n023055ms.ttf", true };
			cfg::uint<0, 1280> margin_x{ this, "Horizontal Margin (px)", 50, true }; // horizontal distance to the screen border relative to the screen_quadrant in px
			cfg::uint<0, 720> margin_y{ this, "Vertical Margin (px)", 50, true }; // vertical distance to the screen border relative to the screen_quadrant in px
			cfg::_bool center_x{ this, "Center Horizontally", false, true };
			cfg::_bool center_y{ this, "Center Vertically", false, true };
			cfg::uint<0, 100> opacity{ this, "Opacity (%)", 70, true };
			cfg::string color_body{ this, "Body Color (hex)", "#FFE138FF", true };
			cfg::string background_body{ this, "Body Background (hex)", "#002339FF", true };
			cfg::string color_title{ this, "Title Color (hex)", "#F26C24FF", true };
			cfg::string background_title{ this, "Title Background (hex)", "#00000000", true };

		} perf_overlay{ this };

		struct node_shader_compilation_hint : cfg::node
		{
			node_shader_compilation_hint(cfg::node* _this) : cfg::node(_this, "Shader Compilation Hint") {}

			cfg::_int<0, 1280> pos_x{ this, "Position X (px)", 20, true }; // horizontal position starting from the upper border in px
			cfg::_int<0, 720> pos_y{ this, "Position Y (px)", 690, true }; // vertical position starting from the left border in px

		} shader_compilation_hint{ this };

		struct node_shader_preloading_dialog : cfg::node
		{
			node_shader_preloading_dialog(cfg::node* _this) : cfg::node(_this, "Shader Loading Dialog") {}

			cfg::_bool use_custom_background{ this, "Allow custom background", true, true };
			cfg::uint<0, 100> darkening_strength{ this, "Darkening effect strength", 30, true };
			cfg::uint<0, 100> blur_strength{ this, "Blur effect strength", 0, true };

		} shader_preloading_dialog{ this };

	} video{ this };

	struct node_audio : cfg::node
	{
		node_audio(cfg::node* _this) : cfg::node(_this, "Audio") {}

		cfg::_enum<audio_renderer> renderer{ this, "Renderer", audio_renderer::cubeb, true };
		cfg::_enum<audio_provider> provider{ this, "Audio Provider", audio_provider::cell_audio, false };
		cfg::_enum<audio_avport> rsxaudio_port{ this, "RSXAudio Avport", audio_avport::hdmi_0, true };
		cfg::_bool dump_to_file{ this, "Dump to file", false, true };
		cfg::_bool convert_to_s16{ this, "Convert to 16 bit", false, true };
		cfg::_enum<audio_format> format{ this, "Audio Format", audio_format::stereo, false };
		cfg::uint<0, umax> formats{ this, "Audio Formats", static_cast<u32>(audio_format_flag::lpcm_2_48khz), false };
		cfg::string audio_device{ this, "Audio Device", "@@@default@@@", true };
		cfg::_int<0, 200> volume{ this, "Master Volume", 100, true };
		cfg::_bool enable_buffering{ this, "Enable Buffering", true, true };
		cfg::_int <4, 250> desired_buffer_duration{ this, "Desired Audio Buffer Duration", 100, true };
		cfg::_bool enable_time_stretching{ this, "Enable Time Stretching", false, true };
		cfg::_bool disable_sampling_skip{ this, "Disable Sampling Skip", false, true };
		cfg::_int<0, 100> time_stretching_threshold{ this, "Time Stretching Threshold", 75, true };
		cfg::_enum<microphone_handler> microphone_type{ this, "Microphone Type", microphone_handler::null };
		cfg::string microphone_devices{ this, "Microphone Devices", "@@@@@@@@@@@@" };
		cfg::_enum<music_handler> music{ this, "Music Handler", music_handler::qt };
	} audio{ this };

	struct node_io : cfg::node
	{
		node_io(cfg::node* _this) : cfg::node(_this, "Input/Output") {}

		cfg::_enum<keyboard_handler> keyboard{ this, "Keyboard", keyboard_handler::null };
		cfg::_enum<mouse_handler> mouse{ this, "Mouse", mouse_handler::basic };
		cfg::_enum<camera_handler> camera{ this, "Camera", camera_handler::null };
		cfg::_enum<fake_camera_type> camera_type{ this, "Camera type", fake_camera_type::unknown };
		cfg::_enum<camera_flip> camera_flip_option{ this, "Camera flip", camera_flip::none, true };
		cfg::string camera_id{ this, "Camera ID", "Default", true };
		cfg::_enum<move_handler> move{ this, "Move", move_handler::null, true };
		cfg::_enum<buzz_handler> buzz{ this, "Buzz emulated controller", buzz_handler::null };
		cfg::_enum<turntable_handler> turntable{this, "Turntable emulated controller", turntable_handler::null};
		cfg::_enum<ghltar_handler> ghltar{this, "GHLtar emulated controller", ghltar_handler::null};
		cfg::_enum<pad_handler_mode> pad_mode{this, "Pad handler mode", pad_handler_mode::single_threaded, true};
		cfg::uint<0, 100'000> pad_sleep{this, "Pad handler sleep (microseconds)", 1'000, true};
		cfg::_bool background_input_enabled{this, "Background input enabled", true, true};
		cfg::_bool show_move_cursor{this, "Show move cursor", false, true};
	} io{ this };

	struct node_sys : cfg::node
	{
		node_sys(cfg::node* _this) : cfg::node(_this, "System") {}

		cfg::_enum<CellSysutilLicenseArea> license_area{ this, "License Area", CellSysutilLicenseArea{1} }; // CELL_SYSUTIL_LICENSE_AREA_A
		cfg::_enum<CellSysutilLang> language{ this, "Language", CellSysutilLang{1} }; // CELL_SYSUTIL_LANG_ENGLISH_US
		cfg::_enum<CellKbMappingType> keyboard_type{ this, "Keyboard Type", CellKbMappingType{0} }; // CELL_KB_MAPPING_101 = US
		cfg::_enum<enter_button_assign> enter_button_assignment{ this, "Enter button assignment", enter_button_assign::cross };
		cfg::_int<-60*60*24*365*100LL, 60*60*24*365*100LL> console_time_offset{ this, "Console time offset (s)", 0 }; // console time offset, limited to +/-100years
		cfg::uint<0,umax> console_psid_high{ this, "PSID high"};
		cfg::uint<0,umax> console_psid_low{ this, "PSID low"};

	} sys{ this };

	struct node_net : cfg::node
	{
		node_net(cfg::node* _this) : cfg::node(_this, "Net") {}

		cfg::_enum<np_internet_status> net_active{this, "Internet enabled", np_internet_status::disabled};
		cfg::string ip_address{this, "IP address", "0.0.0.0"};
		cfg::string bind_address{this, "Bind address", "0.0.0.0"};
		cfg::string dns{this, "DNS address", "8.8.8.8"};
		cfg::string swap_list{this, "IP swap list", ""};

		cfg::_enum<np_psn_status> psn_status{this, "PSN status", np_psn_status::disabled};
	} net{this};

	struct node_savestate : cfg::node
	{
		node_savestate(cfg::node* _this) : cfg::node(_this, "Savestate") {}

		cfg::_bool start_paused{ this, "Start Paused", false }; // Pause on first frame
		cfg::_bool suspend_emu{ this, "Suspend Emulation Savestate Mode", false }; // Close emulation when saving, delete save after loading
		cfg::_bool state_inspection_mode{ this, "Inspection Mode Savestates" }; // Save memory stored in executable files, thus allowing to view state without any files (for debugging)
		cfg::_bool save_disc_game_data{ this, "Save Disc Game Data", false };
	} savestate{this};

	struct node_misc : cfg::node
	{
		node_misc(cfg::node* _this) : cfg::node(_this, "Miscellaneous") {}

		cfg::_bool autostart{ this, "Automatically start games after boot", true, true };
		cfg::_bool autoexit{ this, "Exit RPCS3 when process finishes", false, true };
		cfg::_bool start_fullscreen{ this, "Start games in fullscreen mode", false, true };
		cfg::_bool prevent_display_sleep{ this, "Prevent display sleep while running games", true, true };
		cfg::_bool show_trophy_popups{ this, "Show trophy popups", true, true };
		cfg::_bool show_shader_compilation_hint{ this, "Show shader compilation hint", true, true };
		cfg::_bool use_native_interface{ this, "Use native user interface", true };
		cfg::string gdb_server{ this, "GDB Server", "127.0.0.1:2345" };
		cfg::_bool silence_all_logs{ this, "Silence All Logs", false, true };
		cfg::string title_format{ this, "Window Title Format", "FPS: %F | %R | %V | %T [%t]", true };

	} misc{ this };

	cfg::log_entry log{ this, "Log" };

	std::string name{};
};

extern cfg_root g_cfg;
