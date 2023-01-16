# bot.py
import os
import discord
from discord import app_commands
from discord.ext import commands
from datetime import datetime
from dotenv import load_dotenv
import gspread
from oauth2client.service_account import ServiceAccountCredentials


SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']

load_dotenv()
TOKEN = os.getenv('DISCORD_TOKEN')
SPREADSHEET_ID = os.getenv('SPREADSHEET_ID')

activity = discord.Activity(name='your TCP port', type=discord.ActivityType.listening)
intents = discord.Intents.all()
bot = commands.Bot(command_prefix='$',intents=intents, activity=activity)


@bot.event
async def on_ready():
    print(f'Bot connected as {bot.user}')
    try:
        synced = await bot.tree.sync()
    except Exception as e:
        print(e)
    print('Guild(s): ')
    for guild in bot.guilds:
        print(guild)
    channel = bot.get_channel(1058459522064195644)
    await channel.send(f'{bot.user} is now online!')

    
@bot.tree.error
async def tree_error(interaction: discord.Interaction, error: Exception):
    if isinstance(error, commands.errors.CommandNotFound):
        await interaction.response.send_message('Command not found!', ephemeral=True)
    elif isinstance(error, commands.errors.MissingRequiredArgument):
        await interaction.response.send_message('Missing required argument!', ephemeral=True)
    else:
        await interaction.response.send_message('An error occured!', ephemeral=True)


@bot.tree.command(name="math", description="do some math!")
async def math(interaction: discord.Interaction, args: str):
    if '__' in args:
        await interaction.response.send_message('Nice try hacker!', ephemeral=True)
    elif '**' in args:
        await interaction.response.send_message('This calculator is not powerful enough!', ephemeral=True)
    elif len(args) > 40:
        await interaction.response.send_message('Too many characters!', ephemeral=True)
    else:
        await interaction.response.send_message(eval(args, {'__builtins__': None, 'open': open}), ephemeral=True)


@bot.command()
async def members(ctx):
    await ctx.send(f"{ctx.guild} currently has {len([m for m in ctx.guild.members if not m.bot])} members!")


@bot.tree.command(name="message", description="Adds your message to the spreadsheet!")
@app_commands.describe(message = "Your message")
@app_commands.choices(message = [
                        discord.app_commands.Choice(name='Hello world!', value=1),
                        discord.app_commands.Choice(name='Hi!', value=2),
                        discord.app_commands.Choice(name='Goodbye!', value=3)
                    ])
async def message(interaction: discord.Interaction, message: discord.app_commands.Choice[int]):
    userInfo = [interaction.user.name, message.name, datetime.now().strftime("%m/%d/%Y, %H:%M:%S")]

    scope = ['https://spreadsheets.google.com/feeds',
            'https://www.googleapis.com/auth/drive']
    creds = ServiceAccountCredentials.from_json_keyfile_name('c393263051ec0008915153db5b7e2dda20c42fbe.json', scope)
    client = gspread.authorize(creds)
    sheet = client.open("Display").sheet1
    sheet.append_row(userInfo)
    
    await interaction.response.send_message('Message added!', ephemeral=True)

bot.run(TOKEN)