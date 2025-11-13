import tkinter as tk
from tkinter import messagebox, scrolledtext, ttk, simpledialog
import ctypes
import os
import sys

class VFSApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Virtual File System - GUI with Memory Management")
        self.geometry("1200x800")
        self.configure(bg='#f0f0f0')
        
        # Load the C library
        if not self.load_library():
            return
            
        # Initialize the C backend
        self.initialize_vfs()
        
        # Create status bar FIRST
        self.status_bar = ttk.Label(self, text="Virtual File System Ready", relief=tk.SUNKEN, anchor=tk.W)
        self.status_bar.pack(side=tk.BOTTOM, fill=tk.X)
        
        self.create_notebook()
        self.protocol("WM_DELETE_WINDOW", self.on_closing)
        
    def load_library(self):
        """Load the C shared library"""
        try:
            self.libvfs = ctypes.CDLL('./libvfs.so')
            self.setup_function_prototypes()
            return True
        except OSError as e:
            error_msg = f"Failed to load C library: {e}\n\n"
            error_msg += "Please make sure libvfs.so is in the current directory.\n"
            messagebox.showerror("Library Error", error_msg)
            sys.exit(1)
            
    def setup_function_prototypes(self):
        """Setup function prototypes for ctypes"""
        # Core functions
        self.libvfs.InitialiseSuperB.restype = None
        self.libvfs.CreateDILB.restype = None
        self.libvfs.RestoreData.restype = None
        self.libvfs.BackupData.restype = None
        
        # File operations
        self.libvfs.CreateFile.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.libvfs.CreateFile.restype = ctypes.c_int
        
        self.libvfs.remove_file.argtypes = [ctypes.c_char_p]
        self.libvfs.remove_file.restype = ctypes.c_int
        
        self.libvfs.open_file.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.libvfs.open_file.restype = ctypes.c_int
        
        self.libvfs.close_file.argtypes = [ctypes.c_char_p]
        self.libvfs.close_file.restype = None
        
        self.libvfs.write_file.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
        self.libvfs.write_file.restype = ctypes.c_int
        
        self.libvfs.read_file.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
        self.libvfs.read_file.restype = ctypes.c_int
        
        self.libvfs.truncate_file.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.libvfs.truncate_file.restype = ctypes.c_int
        
        # GUI functions
        self.libvfs.File_ls_gui.restype = ctypes.c_char_p
        self.libvfs.stat_file_gui.argtypes = [ctypes.c_char_p]
        self.libvfs.stat_file_gui.restype = ctypes.c_char_p
        self.libvfs.cat_file_gui.argtypes = [ctypes.c_char_p]
        self.libvfs.cat_file_gui.restype = ctypes.c_char_p
        
        # Memory management functions
        self.libvfs.InitializeMemoryManager.restype = None
        self.libvfs.CreateProcessCLI.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.libvfs.CreateProcessCLI.restype = ctypes.c_int
        self.libvfs.TerminateProcessCLI.argtypes = [ctypes.c_int]
        self.libvfs.TerminateProcessCLI.restype = ctypes.c_int
        self.libvfs.SetAllocAlgorithmCLI.argtypes = [ctypes.c_int]
        self.libvfs.SetAllocAlgorithmCLI.restype = None
        self.libvfs.CompactMemoryCLI.restype = None
        
        # GUI memory functions
        self.libvfs.DisplayMemoryMapGUI.restype = ctypes.c_char_p
        self.libvfs.DisplayProcessListGUI.restype = ctypes.c_char_p
        self.libvfs.MemoryStatsGUI.restype = ctypes.c_char_p
        
        # Utility functions
        self.libvfs.findfd.argtypes = [ctypes.c_char_p]
        self.libvfs.findfd.restype = ctypes.c_int
        
    def initialize_vfs(self):
        """Initialize the virtual file system"""
        self.libvfs.InitialiseSuperB()
        self.libvfs.CreateDILB()
        self.libvfs.RestoreData()
        
    def create_notebook(self):
        """Create notebook with tabs"""
        self.notebook = ttk.Notebook(self)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        self.create_file_tab()
        self.create_memory_tab()
        
        # Initial refresh
        self.refresh_file_list()
        self.refresh_memory_map()
        
    def create_file_tab(self):
        """Create file system tab"""
        file_frame = ttk.Frame(self.notebook)
        self.notebook.add(file_frame, text="File System")
        
        # Left panel - File list
        left_frame = ttk.LabelFrame(file_frame, text="File System Contents", padding="10")
        left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.file_list_text = scrolledtext.ScrolledText(
            left_frame, 
            wrap=tk.WORD, 
            width=50, 
            height=20,
            font=('Courier', 10)
        )
        self.file_list_text.pack(fill=tk.BOTH, expand=True)
        
        # Right panel - Controls
        right_frame = ttk.LabelFrame(file_frame, text="File Operations", padding="10")
        right_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=5, pady=5)
        
        # File information section
        info_frame = ttk.Frame(right_frame)
        info_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(info_frame, text="Filename:").pack(anchor=tk.W)
        self.filename_entry = ttk.Entry(info_frame, width=25)
        self.filename_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(info_frame, text="Permission/Mode (4=R, 2=W, 6=RW):").pack(anchor=tk.W)
        self.permission_entry = ttk.Entry(info_frame, width=10)
        self.permission_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(info_frame, text="Size/Bytes:").pack(anchor=tk.W)
        self.size_entry = ttk.Entry(info_frame, width=10)
        self.size_entry.pack(fill=tk.X, pady=(0, 10))
        
        # Buttons section
        btn_frame = ttk.Frame(right_frame)
        btn_frame.pack(fill=tk.X)
        
        # File operations buttons
        ttk.Button(btn_frame, text="Create File", command=self.create_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Delete File", command=self.delete_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Open File", command=self.open_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Close File", command=self.close_file).pack(fill=tk.X, pady=2)
        
        ttk.Separator(btn_frame, orient='horizontal').pack(fill=tk.X, pady=10)
        
        # Read/Write operations
        ttk.Button(btn_frame, text="Write to File", command=self.write_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Read from File", command=self.read_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Display Content", command=self.cat_file).pack(fill=tk.X, pady=2)
        
        ttk.Separator(btn_frame, orient='horizontal').pack(fill=tk.X, pady=10)
        
        # File information
        ttk.Button(btn_frame, text="File Status", command=self.stat_file).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Truncate File", command=self.truncate_file).pack(fill=tk.X, pady=2)
        
        ttk.Separator(btn_frame, orient='horizontal').pack(fill=tk.X, pady=10)
        
        # System operations
        ttk.Button(btn_frame, text="Refresh List", command=self.refresh_file_list).pack(fill=tk.X, pady=2)
        ttk.Button(btn_frame, text="Backup Data", command=self.backup_data).pack(fill=tk.X, pady=2)
        
    def create_memory_tab(self):
        """Create memory management tab"""
        memory_frame = ttk.Frame(self.notebook)
        self.notebook.add(memory_frame, text="Memory Management")
        
        # Left panel - Memory visualization
        left_mem = ttk.LabelFrame(memory_frame, text="Memory Map", padding="10")
        left_mem.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.memory_text = scrolledtext.ScrolledText(
            left_mem, wrap=tk.WORD, width=60, height=20,
            font=('Courier', 9)
        )
        self.memory_text.pack(fill=tk.BOTH, expand=True)
        
        # Right panel - Controls
        right_mem = ttk.LabelFrame(memory_frame, text="Memory Operations", padding="10")
        right_mem.pack(side=tk.RIGHT, fill=tk.Y, padx=5, pady=5)
        
        # Process creation
        ttk.Label(right_mem, text="Process Name:").pack(anchor=tk.W, pady=(0, 5))
        self.proc_name_entry = ttk.Entry(right_mem, width=20)
        self.proc_name_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(right_mem, text="Memory Required (bytes):").pack(anchor=tk.W, pady=(0, 5))
        self.mem_req_entry = ttk.Entry(right_mem, width=20)
        self.mem_req_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(right_mem, text="Create Process", 
                   command=self.create_process).pack(fill=tk.X, pady=2)
        
        ttk.Label(right_mem, text="Process ID to Terminate:").pack(anchor=tk.W, pady=(20, 5))
        self.proc_kill_entry = ttk.Entry(right_mem, width=20)
        self.proc_kill_entry.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(right_mem, text="Terminate Process", 
                   command=self.terminate_process).pack(fill=tk.X, pady=2)
        
        ttk.Separator(right_mem, orient='horizontal').pack(fill=tk.X, pady=10)
        
        # Algorithm selection
        ttk.Label(right_mem, text="Allocation Algorithm:").pack(anchor=tk.W)
        self.algo_var = tk.StringVar(value="First Fit")
        algo_combo = ttk.Combobox(right_mem, textvariable=self.algo_var,
                                 values=["First Fit", "Best Fit", "Worst Fit"])
        algo_combo.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(right_mem, text="Set Algorithm", 
                   command=self.set_algorithm).pack(fill=tk.X, pady=2)
        
        ttk.Separator(right_mem, orient='horizontal').pack(fill=tk.X, pady=10)
        
        # Information buttons
        ttk.Button(right_mem, text="Initialize Memory", 
                   command=self.initialize_memory).pack(fill=tk.X, pady=2)
        ttk.Button(right_mem, text="Refresh Memory Map", 
                   command=self.refresh_memory_map).pack(fill=tk.X, pady=2)
        ttk.Button(right_mem, text="Show Process List", 
                   command=self.show_process_list).pack(fill=tk.X, pady=2)
        ttk.Button(right_mem, text="Memory Statistics", 
                   command=self.show_memory_stats).pack(fill=tk.X, pady=2)
        ttk.Button(right_mem, text="Compact Memory", 
                   command=self.compact_memory).pack(fill=tk.X, pady=2)
        
    def on_closing(self):
        """Handle application closing"""
        if messagebox.askokcancel("Quit", "Do you want to exit? All data will be backed up."):
            self.libvfs.BackupData()
            self.destroy()
            
    def update_status(self, message):
        """Update status bar"""
        self.status_bar.config(text=message)
        
    # File System Methods
    def refresh_file_list(self):
        """Refresh the file list display"""
        self.file_list_text.delete('1.0', tk.END)
        files_str_ptr = self.libvfs.File_ls_gui()
        if files_str_ptr:
            files_str = ctypes.string_at(files_str_ptr).decode('utf-8')
            self.file_list_text.insert(tk.END, files_str)
        self.update_status("File list refreshed")
        
    def create_file(self):
        """Create a new file"""
        filename = self.filename_entry.get().strip()
        permission_str = self.permission_entry.get().strip()
        
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        if not permission_str.isdigit():
            messagebox.showerror("Error", "Please enter a valid permission (4, 2, or 6)")
            return
            
        permission = int(permission_str)
        if permission not in [4, 2, 6]:
            messagebox.showerror("Error", "Permission must be 4 (READ), 2 (WRITE), or 6 (READ/WRITE)")
            return
            
        ret = self.libvfs.CreateFile(filename.encode('utf-8'), permission)
        if ret >= 0:
            messagebox.showinfo("Success", f"File '{filename}' created successfully.\nFile Descriptor: {ret}")
            self.refresh_file_list()
            self.update_status(f"File '{filename}' created")
        else:
            error_msg = self.get_error_message(ret, "creating")
            messagebox.showerror("Error", error_msg)
            
    def delete_file(self):
        """Delete a file"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename to delete")
            return
            
        if messagebox.askyesno("Confirm", f"Are you sure you want to delete '{filename}'?"):
            ret = self.libvfs.remove_file(filename.encode('utf-8'))
            if ret == 0:
                messagebox.showinfo("Success", f"File '{filename}' deleted successfully")
                self.refresh_file_list()
                self.update_status(f"File '{filename}' deleted")
            else:
                error_msg = self.get_error_message(ret, "deleting")
                messagebox.showerror("Error", error_msg)
                
    def open_file(self):
        """Open a file"""
        filename = self.filename_entry.get().strip()
        permission_str = self.permission_entry.get().strip()
        
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        if not permission_str.isdigit():
            messagebox.showerror("Error", "Please enter a valid mode (4, 2, or 6)")
            return
            
        mode = int(permission_str)
        if mode not in [4, 2, 6]:
            messagebox.showerror("Error", "Mode must be 4 (READ), 2 (WRITE), or 6 (READ/WRITE)")
            return
            
        ret = self.libvfs.open_file(filename.encode('utf-8'), mode)
        if ret >= 0:
            messagebox.showinfo("Success", f"File '{filename}' opened successfully.\nFile Descriptor: {ret}")
            self.update_status(f"File '{filename}' opened with FD: {ret}")
        else:
            error_msg = self.get_error_message(ret, "opening")
            messagebox.showerror("Error", error_msg)
            
    def close_file(self):
        """Close a file"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename to close")
            return
            
        self.libvfs.close_file(filename.encode('utf-8'))
        self.update_status(f"File '{filename}' closed")
        
    def write_file(self):
        """Write data to a file"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        # Get data from user
        data = simpledialog.askstring("Write Data", "Enter data to write:")
        if data is None:
            return
            
        # Find file descriptor
        fd = self.libvfs.findfd(filename.encode('utf-8'))
        if fd < 0:
            messagebox.showerror("Error", f"File '{filename}' not found or not open")
            return
            
        ret = self.libvfs.write_file(fd, data.encode('utf-8'), len(data))
        if ret > 0:
            messagebox.showinfo("Success", f"{ret} bytes written to '{filename}'")
            self.refresh_file_list()
            self.update_status(f"{ret} bytes written to '{filename}'")
        else:
            error_msg = self.get_error_message(ret, "writing")
            messagebox.showerror("Error", error_msg)
            
    def read_file(self):
        """Read data from a file"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        # Get number of bytes to read
        bytes_str = simpledialog.askstring("Read Data", "Number of bytes to read (leave empty for all):")
        if bytes_str is None:
            return
            
        # Find file descriptor
        fd = self.libvfs.findfd(filename.encode('utf-8'))
        if fd < 0:
            messagebox.showerror("Error", f"File '{filename}' not found or not open")
            return
            
        if bytes_str.strip() == "":
            # Read whole file using cat function
            content_ptr = self.libvfs.cat_file_gui(filename.encode('utf-8'))
            if content_ptr:
                content = ctypes.string_at(content_ptr).decode('utf-8')
                self.show_content_dialog(f"Content of {filename}", content)
            else:
                messagebox.showerror("Error", "Failed to read file content")
        else:
            # Read specific number of bytes
            try:
                bytes_to_read = int(bytes_str)
                if bytes_to_read <= 0:
                    messagebox.showerror("Error", "Number of bytes must be positive")
                    return
                    
                buffer = ctypes.create_string_buffer(bytes_to_read + 1)
                ret = self.libvfs.read_file(fd, buffer, bytes_to_read)
                
                if ret > 0:
                    self.show_content_dialog(f"Content of {filename} ({ret} bytes)", buffer.value.decode('utf-8'))
                elif ret == 0:
                    messagebox.showinfo("Info", "End of file reached")
                else:
                    error_msg = self.get_error_message(ret, "reading")
                    messagebox.showerror("Error", error_msg)
                    
            except ValueError:
                messagebox.showerror("Error", "Please enter a valid number")
                
    def cat_file(self):
        """Display file content"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        content_ptr = self.libvfs.cat_file_gui(filename.encode('utf-8'))
        if content_ptr:
            content = ctypes.string_at(content_ptr).decode('utf-8')
            self.show_content_dialog(f"Content of {filename}", content)
            self.update_status(f"Displayed content of '{filename}'")
        else:
            messagebox.showerror("Error", "Failed to read file content")
            
    def stat_file(self):
        """Display file statistics"""
        filename = self.filename_entry.get().strip()
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        stat_ptr = self.libvfs.stat_file_gui(filename.encode('utf-8'))
        if stat_ptr:
            stat_str = ctypes.string_at(stat_ptr).decode('utf-8')
            self.show_content_dialog(f"Statistics of {filename}", stat_str)
            self.update_status(f"Displayed stats for '{filename}'")
        else:
            messagebox.showerror("Error", "Failed to get file statistics")
            
    def truncate_file(self):
        """Truncate a file"""
        filename = self.filename_entry.get().strip()
        size_str = self.size_entry.get().strip()
        
        if not filename:
            messagebox.showerror("Error", "Please enter a filename")
            return
            
        if not size_str.isdigit():
            messagebox.showerror("Error", "Please enter a valid size")
            return
            
        size = int(size_str)
        ret = self.libvfs.truncate_file(filename.encode('utf-8'), size)
        if ret == 0:
            messagebox.showinfo("Success", f"File '{filename}' truncated to {size} bytes")
            self.refresh_file_list()
            self.update_status(f"File '{filename}' truncated to {size} bytes")
        else:
            error_msg = self.get_error_message(ret, "truncating")
            messagebox.showerror("Error", error_msg)
            
    def backup_data(self):
        """Backup all data"""
        self.libvfs.BackupData()
        messagebox.showinfo("Success", "All data backed up successfully")
        self.update_status("Data backed up")
        
    # Memory Management Methods
    def initialize_memory(self):
        """Initialize memory manager"""
        self.libvfs.InitializeMemoryManager()
        self.refresh_memory_map()
        self.update_status("Memory manager initialized")
        messagebox.showinfo("Success", "Memory Manager Initialized with 1MB total memory")
        
    def create_process(self):
        """Create a new process"""
        proc_name = self.proc_name_entry.get().strip()
        mem_str = self.mem_req_entry.get().strip()
        
        if not proc_name:
            messagebox.showerror("Error", "Please enter a process name")
            return
            
        if not mem_str.isdigit():
            messagebox.showerror("Error", "Please enter valid memory size")
            return
            
        memory = int(mem_str)
        if memory <= 0:
            messagebox.showerror("Error", "Memory size must be positive")
            return
            
        pid = self.libvfs.CreateProcessCLI(proc_name.encode('utf-8'), memory)
        if pid > 0:
            messagebox.showinfo("Success", f"Process '{proc_name}' created with PID: {pid}")
            self.refresh_memory_map()
            self.update_status(f"Process '{proc_name}' created with PID {pid}")
        else:
            messagebox.showerror("Error", "Failed to create process (not enough memory?)")
            
    def terminate_process(self):
        """Terminate a process"""
        pid_str = self.proc_kill_entry.get().strip()
        
        if not pid_str.isdigit():
            messagebox.showerror("Error", "Please enter a valid process ID")
            return
            
        pid = int(pid_str)
        ret = self.libvfs.TerminateProcessCLI(pid)
        if ret > 0:
            messagebox.showinfo("Success", f"Process {pid} terminated, freed {ret} bytes")
            self.refresh_memory_map()
            self.update_status(f"Process {pid} terminated")
        else:
            messagebox.showerror("Error", f"Failed to terminate process {pid}")
            
    def set_algorithm(self):
        """Set allocation algorithm"""
        algo_map = {
            "First Fit": 0,
            "Best Fit": 1,
            "Worst Fit": 2
        }
        
        algo_name = self.algo_var.get()
        if algo_name in algo_map:
            self.libvfs.SetAllocAlgorithmCLI(algo_map[algo_name])
            self.refresh_memory_map()
            self.update_status(f"Algorithm set to {algo_name}")
        else:
            messagebox.showerror("Error", "Please select a valid algorithm")
            
    def refresh_memory_map(self):
        """Refresh memory map display"""
        self.memory_text.delete('1.0', tk.END)
        mem_str_ptr = self.libvfs.DisplayMemoryMapGUI()
        if mem_str_ptr:
            mem_str = ctypes.string_at(mem_str_ptr).decode('utf-8')
            self.memory_text.insert(tk.END, mem_str)
        self.update_status("Memory map refreshed")
        
    def show_process_list(self):
        """Show process list in dialog"""
        proc_str_ptr = self.libvfs.DisplayProcessListGUI()
        if proc_str_ptr:
            proc_str = ctypes.string_at(proc_str_ptr).decode('utf-8')
            self.show_content_dialog("Process List", proc_str)
        self.update_status("Process list displayed")
        
    def show_memory_stats(self):
        """Show memory statistics"""
        stats_ptr = self.libvfs.MemoryStatsGUI()
        if stats_ptr:
            stats_str = ctypes.string_at(stats_ptr).decode('utf-8')
            self.show_content_dialog("Memory Statistics", stats_str)
        self.update_status("Memory statistics displayed")
        
    def compact_memory(self):
        """Compact memory"""
        self.libvfs.CompactMemoryCLI()
        self.refresh_memory_map()
        self.update_status("Memory compaction completed")
        
    def show_content_dialog(self, title, content):
        """Show content in a scrollable dialog"""
        dialog = tk.Toplevel(self)
        dialog.title(title)
        dialog.geometry("600x400")
        
        text_widget = scrolledtext.ScrolledText(dialog, wrap=tk.WORD)
        text_widget.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        text_widget.insert(tk.END, content)
        text_widget.config(state=tk.DISABLED)
        
        ttk.Button(dialog, text="Close", command=dialog.destroy).pack(pady=10)
        
    def get_error_message(self, error_code, operation):
        """Convert error code to human readable message"""
        error_messages = {
            -1: "Invalid parameter",
            -2: "File not found",
            -3: "No space available",
            -4: "Permission denied", 
            -5: "File already exists",
            -6: "Invalid file descriptor",
            -7: "File is busy"
        }
        
        return f"Error {operation} file: {error_messages.get(error_code, 'Unknown error')}"

if __name__ == "__main__":
    app = VFSApp()
    app.mainloop()
