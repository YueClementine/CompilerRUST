fn main()
{
	let mut var1=2;
	let mut var2=10;

	if var2 < var1*2 {
		println!("{}",var2);
	}
	else {
		let mut temp=var2;
		var2 = var1;
		var1 = temp;
		println!("{}",var1);
		println!("{}",var2);
	}

}